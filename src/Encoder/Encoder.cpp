#include "Encoder.h"
#include "../Utils/Logger.h"
#include <chrono>
#include <algorithm>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

Encoder::Encoder() 
    : state_(EncoderState::Uninitialized)
    , startTime_(std::chrono::steady_clock::now()) {
    LOG_DEBUG("Encoder created");
}

Encoder::~Encoder() {
    if (state_ != EncoderState::Closed) {
        close();
    }
    LOG_DEBUG("Encoder destroyed");
}

bool Encoder::init(const EncoderConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (state_ != EncoderState::Uninitialized) {
        LOG_ERROR("Encoder already initialized");
        return false;
    }
    
    if (config.outputPath.empty()) {
        LOG_ERROR("Output path is empty");
        return false;
    }
    
    config_ = config;
    
    LOG_INFO("Initializing encoder with output: " + config_.outputPath);
    LOG_INFO("Video: " + config_.videoCodec + " " + 
             std::to_string(config_.videoWidth) + "x" + std::to_string(config_.videoHeight) + 
             " @ " + std::to_string(config_.videoBitrate/1000) + "kbps");
    LOG_INFO("Audio: " + config_.audioCodec + " " + 
             std::to_string(config_.audioSampleRate) + "Hz " + 
             std::to_string(config_.audioChannels) + "ch @ " + 
             std::to_string(config_.audioBitrate/1000) + "kbps");
    
    if (!initializeComponents()) {
        LOG_ERROR("Failed to initialize encoder components");
        return false;
    }
    
    setState(EncoderState::Running);
    startTime_ = std::chrono::steady_clock::now();
    
    LOG_INFO("Encoder initialized successfully");
    return true;
}

bool Encoder::push(const FrameVariant& frame) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!validateState(EncoderState::Running)) {
        return false;
    }
    
    bool success = false;
    
    if (std::holds_alternative<FrameData>(frame)) {
        const auto& videoFrame = std::get<FrameData>(frame);
        success = processVideoFrame(videoFrame);
        if (success) {
            stats_.videoFramesEncoded++;
        }
    } else if (std::holds_alternative<AudioFrameData>(frame)) {
        const auto& audioFrame = std::get<AudioFrameData>(frame);
        success = processAudioFrame(audioFrame);
        if (success) {
            stats_.audioFramesEncoded++;
        }
    } else {
        LOG_ERROR("Unknown frame type");
        return false;
    }
    
    if (success) {
        updateStatistics();
    }
    
    return success;
}

bool Encoder::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!validateState(EncoderState::Running)) {
        return false;
    }
    
    LOG_INFO("Flushing encoders...");
    
    bool success = true;
    
    // 刷新视频编码器
    if (videoEncoder_) {
        std::vector<AVPacket*> packets;
        if (videoEncoder_->flush(packets)) {
            for (auto* packet : packets) {
                if (!muxer_->writePacket(packet, 0)) {  // 假设视频流索引为0
                    success = false;
                }
                av_packet_free(&packet);
            }
        } else {
            success = false;
        }
    }
    
    // 刷新音频编码器
    if (audioEncoder_) {
        std::vector<AVPacket*> packets;
        if (audioEncoder_->flush(packets)) {
            for (auto* packet : packets) {
                if (!muxer_->writePacket(packet, 1)) {  // 假设音频流索引为1
                    success = false;
                }
                av_packet_free(&packet);
            }
        } else {
            success = false;
        }
    }
    
    if (success) {
        setState(EncoderState::Flushed);
        LOG_INFO("Encoders flushed successfully");
    } else {
        LOG_ERROR("Failed to flush encoders");
    }
    
    return success;
}

bool Encoder::close() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (state_ == EncoderState::Closed) {
        return true;
    }
    
    LOG_INFO("Closing encoder...");
    
    // 如果还在运行状态，先刷新
    if (state_ == EncoderState::Running) {
        flush();
    }
    
    // 关闭封装器
    if (muxer_) {
        muxer_->finalize();
        muxer_.reset();
    }
    
    // 关闭编码器
    if (videoEncoder_) {
        videoEncoder_->close();
        videoEncoder_.reset();
    }
    
    if (audioEncoder_) {
        audioEncoder_->close();
        audioEncoder_.reset();
    }
    
    setState(EncoderState::Closed);
    
    // 输出最终统计信息
    auto finalStats = getStatistics();
    LOG_INFO("Encoding completed:");
    LOG_INFO("  Video frames: " + std::to_string(finalStats.videoFramesEncoded));
    LOG_INFO("  Audio frames: " + std::to_string(finalStats.audioFramesEncoded));
    LOG_INFO("  Total duration: " + std::to_string(finalStats.totalDuration) + "s");
    LOG_INFO("  Encoding speed: " + std::to_string(finalStats.encodingSpeed) + "x");
    LOG_INFO("  Output file: " + config_.outputPath);
    
    return true;
}

EncoderState Encoder::getState() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return state_;
}

Encoder::Statistics Encoder::getStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto currentStats = stats_;
    
    // 计算编码速度
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - startTime_).count() / 1000.0;
    
    if (elapsed > 0 && currentStats.totalDuration > 0) {
        currentStats.encodingSpeed = currentStats.totalDuration / elapsed;
    }
    
    return currentStats;
}

bool Encoder::isCodecSupported(const std::string& codecName, bool isVideo) {
    const AVCodec* codec = avcodec_find_encoder_by_name(codecName.c_str());
    if (!codec) {
        return false;
    }
    
    if (isVideo) {
        return codec->type == AVMEDIA_TYPE_VIDEO;
    } else {
        return codec->type == AVMEDIA_TYPE_AUDIO;
    }
}

std::vector<std::string> Encoder::getSupportedCodecs(bool isVideo) {
    std::vector<std::string> codecs;
    
    const AVCodec* codec = nullptr;
    void* iter = nullptr;
    
    while ((codec = av_codec_iterate(&iter))) {
        if (av_codec_is_encoder(codec)) {
            if ((isVideo && codec->type == AVMEDIA_TYPE_VIDEO) ||
                (!isVideo && codec->type == AVMEDIA_TYPE_AUDIO)) {
                codecs.push_back(codec->name);
            }
        }
    }
    
    std::sort(codecs.begin(), codecs.end());
    return codecs;
}

bool Encoder::initializeComponents() {
    // 创建封装器
    muxer_ = std::make_unique<Muxer>();
    MuxerConfig muxerConfig;
    muxerConfig.outputPath = config_.outputPath;
    muxerConfig.format = config_.format;
    muxerConfig.fastStart = config_.fastStart;
    
    if (!muxer_->init(muxerConfig)) {
        LOG_ERROR("Failed to initialize muxer");
        return false;
    }
    
    // 创建视频编码器
    videoEncoder_ = std::make_unique<VideoEncoder>();
    VideoEncoderConfig videoConfig;
    videoConfig.codec = config_.videoCodec;
    videoConfig.width = config_.videoWidth;
    videoConfig.height = config_.videoHeight;
    videoConfig.bitrate = config_.videoBitrate;
    videoConfig.frameRate = config_.videoFrameRate;
    videoConfig.preset = config_.videoPreset;
    videoConfig.enableHardwareAccel = config_.enableHardwareAccel;
    videoConfig.threadCount = config_.threadCount;
    videoConfig.crf = config_.videoCRF;
    
                AVRational videoTimeBase = {1, static_cast<int>(config_.videoFrameRate)};
    if (!videoEncoder_->init(videoConfig, videoTimeBase)) {
        LOG_ERROR("Failed to initialize video encoder");
        return false;
    }
    
    // 创建音频编码器（如果需要）
    bool hasAudio = (config_.audioSampleRate > 0 && config_.audioChannels > 0);
    if (hasAudio) {
        audioEncoder_ = std::make_unique<AudioEncoder>();
        AudioEncoderConfig audioConfig;
        audioConfig.codec = config_.audioCodec;
        audioConfig.sampleRate = config_.audioSampleRate;
        audioConfig.channels = config_.audioChannels;
        audioConfig.bitrate = config_.audioBitrate;
        
        AVRational audioTimeBase = {1, config_.audioSampleRate};
        if (!audioEncoder_->init(audioConfig, audioTimeBase)) {
            LOG_ERROR("Failed to initialize audio encoder");
            return false;
        }
        LOG_INFO("Audio encoder initialized");
    } else {
        LOG_INFO("Skipping audio encoder (video only mode)");
    }
    
    // 添加流到封装器
    if (muxer_->addVideoStream(videoEncoder_->getCodecContext()) < 0) {
        LOG_ERROR("Failed to add video stream to muxer");
        return false;
    }
    
    // 只在有音频编码器时添加音频流
    if (audioEncoder_) {
        if (muxer_->addAudioStream(audioEncoder_->getCodecContext()) < 0) {
            LOG_ERROR("Failed to add audio stream to muxer");
            return false;
        }
    }
    
    // 写入文件头
    if (!muxer_->writeHeader()) {
        LOG_ERROR("Failed to write file header");
        return false;
    }
    
    return true;
}

bool Encoder::processVideoFrame(const FrameData& frame) {
    if (!videoEncoder_) {
        LOG_ERROR("Video encoder not initialized");
        return false;
    }
    
    // 如果是第一帧且配置中尺寸为0，则从帧中获取尺寸
    if (config_.videoWidth == 0 || config_.videoHeight == 0) {
        if (frame.width > 0 && frame.height > 0) {
            LOG_INFO("Auto-detecting video size from frame: " + 
                     std::to_string(frame.width) + "x" + std::to_string(frame.height));
            config_.videoWidth = frame.width;
            config_.videoHeight = frame.height;
            
            // 重新初始化视频编码器
            videoEncoder_->close();
            VideoEncoderConfig videoConfig;
            videoConfig.codec = config_.videoCodec;
            videoConfig.width = config_.videoWidth;
            videoConfig.height = config_.videoHeight;
            videoConfig.bitrate = config_.videoBitrate;
            videoConfig.frameRate = config_.videoFrameRate;
            videoConfig.preset = config_.videoPreset;
            videoConfig.enableHardwareAccel = config_.enableHardwareAccel;
            videoConfig.threadCount = config_.threadCount;
            videoConfig.crf = config_.videoCRF;
            
            AVRational videoTimeBase = {1, static_cast<int>(config_.videoFrameRate)};
            if (!videoEncoder_->init(videoConfig, videoTimeBase)) {
                LOG_ERROR("Failed to reinitialize video encoder with detected size");
                return false;
            }
            
            // 更新封装器中的视频流
            if (muxer_->addVideoStream(videoEncoder_->getCodecContext()) < 0) {
                LOG_ERROR("Failed to update video stream in muxer");
                return false;
            }
        } else {
            LOG_ERROR("Invalid frame size: " + std::to_string(frame.width) + "x" + std::to_string(frame.height));
            return false;
        }
    }
    
    std::vector<AVPacket*> packets;
    if (!videoEncoder_->encode(frame, packets)) {
        LOG_ERROR("Failed to encode video frame");
        return false;
    }
    
    // 写入封装器
    for (auto* packet : packets) {
        if (!muxer_->writePacket(packet, 0)) {  // 视频流索引为0
            LOG_ERROR("Failed to write video packet");
            av_packet_free(&packet);
            return false;
        }
        av_packet_free(&packet);
    }
    
    return true;
}

bool Encoder::processAudioFrame(const AudioFrameData& frame) {
    if (!audioEncoder_) {
        LOG_ERROR("Audio encoder not initialized");
        return false;
    }
    
    std::vector<AVPacket*> packets;
    if (!audioEncoder_->encode(frame, packets)) {
        LOG_ERROR("Failed to encode audio frame");
        return false;
    }
    
    // 写入封装器
    for (auto* packet : packets) {
        if (!muxer_->writePacket(packet, 1)) {  // 音频流索引为1
            LOG_ERROR("Failed to write audio packet");
            av_packet_free(&packet);
            return false;
        }
        av_packet_free(&packet);
    }
    
    return true;
}

void Encoder::updateStatistics() {
    // 从子组件获取统计信息
    if (videoEncoder_) {
        auto videoStats = videoEncoder_->getStatistics();
        stats_.totalBytesWritten += videoStats.bytesEncoded;
    }
    
    if (audioEncoder_) {
        auto audioStats = audioEncoder_->getStatistics();
        stats_.totalBytesWritten += audioStats.bytesEncoded;
    }
    
    if (muxer_) {
        auto muxerStats = muxer_->getStatistics();
        stats_.totalDuration = muxerStats.totalDuration;
    }
}

void Encoder::setState(EncoderState newState) {
    state_ = newState;
    LOG_DEBUG("Encoder state changed to: " + std::to_string(static_cast<int>(newState)));
}

bool Encoder::validateState(EncoderState requiredState) const {
    if (state_ != requiredState) {
        LOG_ERROR("Invalid encoder state. Required: " + 
                  std::to_string(static_cast<int>(requiredState)) + 
                  ", Current: " + std::to_string(static_cast<int>(state_)));
        return false;
    }
    return true;
}
