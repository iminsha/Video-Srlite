#include "AudioEncoder.h"
#include "../Utils/Logger.h"
#include <chrono>

AudioEncoder::AudioEncoder()
    : codec_(nullptr)
    , codecContext_(nullptr)
    , frame_(nullptr)
    , swrContext_(nullptr)
    , initialized_(false)
    , sampleIndex_(0)
    , resampleBuffer_(nullptr)
    , resampleBufferSize_(0) {
    LOG_DEBUG("AudioEncoder created");
}

AudioEncoder::~AudioEncoder() {
    close();
    LOG_DEBUG("AudioEncoder destroyed");
}

bool AudioEncoder::init(const AudioEncoderConfig& config, AVRational timeBase) {
    if (initialized_) {
        LOG_ERROR("AudioEncoder already initialized");
        return false;
    }
    
    config_ = config;
    timeBase_ = timeBase;
    
    LOG_INFO("Initializing audio encoder: " + config_.codec);
    LOG_INFO("Sample rate: " + std::to_string(config_.sampleRate) + "Hz");
    LOG_INFO("Channels: " + std::to_string(config_.channels));
    LOG_INFO("Bitrate: " + std::to_string(config_.bitrate/1000) + "kbps");
    
    if (config_.enableStreamCopy) {
        LOG_INFO("Stream copy mode enabled");
        initialized_ = true;
        return true;
    }
    
    if (!setupCodec()) {
        LOG_ERROR("Failed to setup codec");
        return false;
    }
    
    initialized_ = true;
    sampleIndex_ = 0;
    
    LOG_INFO("AudioEncoder initialized successfully");
    return true;
}

bool AudioEncoder::encode(const AudioFrameData& frameData, std::vector<AVPacket*>& packets) {
    if (!initialized_) {
        LOG_ERROR("AudioEncoder not initialized");
        return false;
    }
    
    // 流拷贝模式
    if (config_.enableStreamCopy) {
        return processStreamCopy(frameData, packets);
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 转换音频帧数据
    AVFrame* avFrame = convertAudioFrameData(frameData);
    if (!avFrame) {
        LOG_ERROR("Failed to convert audio frame data");
        return false;
    }
    
    // 设置时间戳
    avFrame->pts = sampleIndex_;
    sampleIndex_ += avFrame->nb_samples;
    
    // 发送帧到编码器
    int ret = avcodec_send_frame(codecContext_, avFrame);
    if (ret < 0) {
        LOG_ERROR("Error sending frame to encoder: " + std::to_string(ret));
        av_frame_free(&avFrame);
        return false;
    }
    
    // 接收编码包
    while (ret >= 0) {
        AVPacket* packet = av_packet_alloc();
        if (!packet) {
            LOG_ERROR("Failed to allocate packet");
            av_frame_free(&avFrame);
            return false;
        }
        
        ret = avcodec_receive_packet(codecContext_, packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_packet_free(&packet);
            break;
        } else if (ret < 0) {
            LOG_ERROR("Error receiving packet from encoder: " + std::to_string(ret));
            av_packet_free(&packet);
            av_frame_free(&avFrame);
            return false;
        }
        
        packets.push_back(packet);
        
        // 更新统计信息
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        updateStatistics(duration.count() / 1000.0, packet->size, avFrame->nb_samples);
    }
    
    av_frame_free(&avFrame);
    return true;
}

bool AudioEncoder::flush(std::vector<AVPacket*>& packets) {
    if (!initialized_ || config_.enableStreamCopy) {
        return true;  // 流拷贝模式不需要刷新
    }
    
    LOG_DEBUG("Flushing audio encoder");
    
    // 发送空帧表示结束
    int ret = avcodec_send_frame(codecContext_, nullptr);
    if (ret < 0) {
        LOG_ERROR("Error sending flush frame to encoder: " + std::to_string(ret));
        return false;
    }
    
    // 接收剩余的编码包
    while (ret >= 0) {
        AVPacket* packet = av_packet_alloc();
        if (!packet) {
            LOG_ERROR("Failed to allocate packet");
            return false;
        }
        
        ret = avcodec_receive_packet(codecContext_, packet);
        if (ret == AVERROR_EOF) {
            av_packet_free(&packet);
            break;
        } else if (ret < 0) {
            LOG_ERROR("Error receiving packet from encoder: " + std::to_string(ret));
            av_packet_free(&packet);
            return false;
        }
        
        packets.push_back(packet);
        updateStatistics(0.0, packet->size, 0);
    }
    
    LOG_DEBUG("Audio encoder flushed, " + std::to_string(packets.size()) + " packets");
    return true;
}

void AudioEncoder::close() {
    freeResampleBuffer();
    
    if (swrContext_) {
        swr_free(&swrContext_);
    }
    
    if (frame_) {
        av_frame_free(&frame_);
    }
    
    if (codecContext_) {
        avcodec_free_context(&codecContext_);
    }
    
    initialized_ = false;
    LOG_DEBUG("AudioEncoder closed");
}

AudioEncoder::Statistics AudioEncoder::getStatistics() const {
    return stats_;
}

bool AudioEncoder::setupCodec() {
    // 查找编码器
    codec_ = avcodec_find_encoder_by_name(config_.codec.c_str());
    if (!codec_) {
        LOG_ERROR("Codec not found: " + config_.codec);
        return false;
    }
    
    // 分配编码器上下文
    codecContext_ = avcodec_alloc_context3(codec_);
    if (!codecContext_) {
        LOG_ERROR("Failed to allocate codec context");
        return false;
    }
    
    // 设置编码参数
    codecContext_->sample_rate = config_.sampleRate;
    codecContext_->bit_rate = config_.bitrate;
    codecContext_->sample_fmt = config_.sampleFormat;
    codecContext_->time_base = timeBase_;
    
    // 设置声道布局
    codecContext_->channel_layout = config_.channelLayout;
    codecContext_->channels = av_get_channel_layout_nb_channels(config_.channelLayout);
    
    // 设置帧大小
    if (codec_->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) {
        codecContext_->frame_size = config_.frameSize;
    }
    
    // 打开编码器
    int ret = avcodec_open2(codecContext_, codec_, nullptr);
    if (ret < 0) {
        LOG_ERROR("Failed to open codec: " + std::to_string(ret));
        return false;
    }
    
    // 分配帧
    frame_ = av_frame_alloc();
    if (!frame_) {
        LOG_ERROR("Failed to allocate frame");
        return false;
    }
    
    frame_->format = codecContext_->sample_fmt;
    frame_->nb_samples = codecContext_->frame_size;
    frame_->channel_layout = codecContext_->channel_layout;
    frame_->channels = codecContext_->channels;
    
    ret = av_frame_get_buffer(frame_, 0);
    if (ret < 0) {
        LOG_ERROR("Failed to allocate frame buffer: " + std::to_string(ret));
        return false;
    }
    
    return true;
}

AVFrame* AudioEncoder::convertAudioFrameData(const AudioFrameData& frameData) {
    if (frameData.data.empty()) {
        LOG_ERROR("Input audio data is empty");
        return nullptr;
    }
    
    // 检查是否需要重采样
    if (!needsResampling(frameData)) {
        // 直接使用原始数据（简化实现）
        if (av_frame_make_writable(frame_) < 0) {
            LOG_ERROR("Failed to make frame writable");
            return nullptr;
        }
        
        // 简化实现：直接拷贝数据
        size_t dataSize = std::min(frameData.data.size(), 
                                  static_cast<size_t>(frame_->nb_samples * frame_->channels * sizeof(float)));
        memcpy(frame_->data[0], frameData.data.data(), dataSize);
        
        return frame_;
    }
    
    // 需要重采样
    if (!setupSwrContext()) {
        LOG_ERROR("Failed to setup SwrContext");
        return nullptr;
    }
    
    // 进行重采样（简化实现）
    if (!resampleAudio(frameData, frame_)) {
        LOG_ERROR("Failed to resample audio");
        return nullptr;
    }
    
    return frame_;
}

bool AudioEncoder::setupSwrContext() {
    if (swrContext_) {
        return true;  // 已经设置过了
    }
    
    // 创建重采样上下文
    swrContext_ = swr_alloc_set_opts(nullptr,
                                    config_.channelLayout, config_.sampleFormat, config_.sampleRate,
                                    config_.channelLayout, AV_SAMPLE_FMT_FLT, config_.sampleRate,
                                 0, nullptr);
    
    if (!swrContext_) {
        LOG_ERROR("Failed to allocate SwrContext");
        return false;
    }
    
    int ret = swr_init(swrContext_);
    if (ret < 0) {
        LOG_ERROR("Failed to initialize SwrContext: " + std::to_string(ret));
        return false;
    }
    
    return true;
}

bool AudioEncoder::resampleAudio(const AudioFrameData& frameData, AVFrame* outputFrame) {
    // 简化实现：假设不需要重采样，直接拷贝
    if (av_frame_make_writable(outputFrame) < 0) {
        LOG_ERROR("Failed to make output frame writable");
        return false;
    }
    
    size_t dataSize = std::min(frameData.data.size(), 
                              static_cast<size_t>(outputFrame->nb_samples * outputFrame->channels * sizeof(float)));
    memcpy(outputFrame->data[0], frameData.data.data(), dataSize);
    
    return true;
}

void AudioEncoder::updateStatistics(double encodingTime, size_t packetSize, int samples) {
    stats_.framesEncoded++;
    stats_.bytesEncoded += packetSize;
    stats_.samplesEncoded += samples;
    
    // 更新平均编码时间
    if (stats_.framesEncoded > 0) {
        stats_.avgEncodingTime = (stats_.avgEncodingTime * (stats_.framesEncoded - 1) + encodingTime) / stats_.framesEncoded;
    }
}

void AudioEncoder::freeResampleBuffer() {
    if (resampleBuffer_) {
        av_freep(&resampleBuffer_[0]);
        av_freep(&resampleBuffer_);
        resampleBufferSize_ = 0;
    }
}

AVSampleFormat AudioEncoder::getAVSampleFormat(const std::string& format) const {
    if (format == "s16le") return AV_SAMPLE_FMT_S16;
    if (format == "fltp") return AV_SAMPLE_FMT_FLTP;
    if (format == "flt") return AV_SAMPLE_FMT_FLT;
    if (format == "s32") return AV_SAMPLE_FMT_S32;
    
    // 默认返回浮点格式
    return AV_SAMPLE_FMT_FLTP;
}

uint64_t AudioEncoder::getAVChannelLayout(int channels) const {
    switch (channels) {
        case 1: return AV_CH_LAYOUT_MONO;
        case 2: return AV_CH_LAYOUT_STEREO;
        case 6: return AV_CH_LAYOUT_5POINT1;
        case 8: return AV_CH_LAYOUT_7POINT1;
        default: return AV_CH_LAYOUT_STEREO;
    }
}

bool AudioEncoder::needsResampling(const AudioFrameData& frameData) const {
    // 检查采样率
    if (frameData.sampleRate != config_.sampleRate) {
        return true;
    }
    
    // 检查声道数
    if (frameData.channels != config_.channels) {
        return true;
    }
    
    // 检查样本格式
    AVSampleFormat inputFormat = getAVSampleFormat(frameData.format);
    if (inputFormat != config_.sampleFormat) {
        return true;
    }
    
    return false;
}

bool AudioEncoder::processStreamCopy(const AudioFrameData& frameData, std::vector<AVPacket*>& packets) {
    // 流拷贝模式：直接将音频数据封装为AVPacket
    AVPacket* packet = av_packet_alloc();
    if (!packet) {
        LOG_ERROR("Failed to allocate packet for stream copy");
        return false;
    }
    
    // 分配数据
    int ret = av_new_packet(packet, frameData.data.size());
    if (ret < 0) {
        LOG_ERROR("Failed to allocate packet data: " + std::to_string(ret));
        av_packet_free(&packet);
        return false;
    }
    
    // 拷贝数据
    memcpy(packet->data, frameData.data.data(), frameData.data.size());
    
    // 设置时间戳
    packet->pts = sampleIndex_;
    packet->dts = sampleIndex_;
    sampleIndex_ += frameData.nbSamples;
    
    packets.push_back(packet);
    
    // 更新统计信息
    updateStatistics(0.0, packet->size, frameData.nbSamples);
    
    return true;
}
