#include "../include/AudioDecoder.h"
#include "../../Utils/Logger.h"
#include <cstring>

extern "C" {
#include <libavutil/opt.h>
}

AudioDecoder::AudioDecoder() {
    // FFmpeg初始化在第一次使用时进行
}

AudioDecoder::~AudioDecoder() {
    destroy();
}

bool AudioDecoder::initialize(const AudioDecoderConfig& config) {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    if (initialized_) {
        return true;
    }
    
    config_ = config;
    
    // 分配FFmpeg结构
    packet_ = av_packet_alloc();
    frame_ = av_frame_alloc();
    
    if (!packet_ || !frame_) {
        LOG_ERROR("无法分配FFmpeg结构");
        cleanup();
        return false;
    }
    
    initialized_ = true;
    LOG_DEBUG("AudioDecoder初始化成功");
    return true;
}

void AudioDecoder::destroy() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    if (!initialized_) return;
    
    LOG_DEBUG("开始销毁音频解码器...");
    
    // 1. 首先关闭文件（如果已打开）
    if (opened_) {
        // 临时释放锁，避免在close()中重复加锁
        if (threadSafe_) lock.unlock();
        close();
        if (threadSafe_) lock.lock();
    }
    
    // 2. 清理基本资源
    cleanup();
    
    // 3. 重置状态
    initialized_ = false;
    
    LOG_DEBUG("AudioDecoder已销毁");
}

bool AudioDecoder::open(const std::string& filepath) {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    if (!initialized_) {
        if (!initialize()) {
            return false;
        }
    }
    
    if (opened_) {
        close();
    }
    
    // 打开文件
    if (avformat_open_input(&formatCtx_, filepath.c_str(), nullptr, nullptr) < 0) {
        LOG_ERROR("无法打开音频文件: " + filepath);
        return false;
    }
    
    // 查找流信息
    if (avformat_find_stream_info(formatCtx_, nullptr) < 0) {
        LOG_ERROR("无法找到流信息");
        avformat_close_input(&formatCtx_);
        return false;
    }
    
    // 查找音频流
    audioStreamIndex_ = -1;
    for (unsigned int i = 0; i < formatCtx_->nb_streams; i++) {
        if (formatCtx_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex_ = i;
            break;
        }
    }
    
    if (audioStreamIndex_ == -1) {
        LOG_WARNING("未找到音频流");
        avformat_close_input(&formatCtx_);
        return false;
    }
    
    if (!initDecoder()) {
        avformat_close_input(&formatCtx_);
        return false;
    }
    
    // 填充音频信息
    AVStream* stream = formatCtx_->streams[audioStreamIndex_];
    audioInfo_.sampleRate = codecCtx_->sample_rate;
    audioInfo_.channels = codecCtx_->channels;
    audioInfo_.duration = formatCtx_->duration / (double)AV_TIME_BASE;
    audioInfo_.totalSamples = stream->nb_frames;
    audioInfo_.codecName = avcodec_get_name(codecCtx_->codec_id);
    audioInfo_.sampleFormat = av_get_sample_fmt_name(codecCtx_->sample_fmt);
    audioInfo_.bitRate = codecCtx_->bit_rate;
    
    opened_ = true;
    currentTime_ = 0.0;
    currentSample_ = 0;
    
    LOG_INFO("音频解码器打开成功: " + std::to_string(audioInfo_.sampleRate) + "Hz " + 
             std::to_string(audioInfo_.channels) + "ch");
    
    return true;
}

void AudioDecoder::close() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    if (!opened_) return;
    
    LOG_DEBUG("开始关闭音频解码器...");
    
    // 1. 首先关闭SwrContext
    if (swrCtx_) {
        swr_free(&swrCtx_);
        swrCtx_ = nullptr;
        LOG_DEBUG("SwrContext已释放");
    }
    
    // 2. 关闭编解码器上下文
    if (codecCtx_) {
        avcodec_free_context(&codecCtx_);
        codecCtx_ = nullptr;
        LOG_DEBUG("AudioCodecContext已释放");
    }
    
    // 3. 最后关闭格式上下文
    if (formatCtx_) {
        avformat_close_input(&formatCtx_);
        formatCtx_ = nullptr;
        LOG_DEBUG("AudioFormatContext已释放");
    }
    
    // 4. 重置状态
    audioStreamIndex_ = -1;
    opened_ = false;
    currentTime_ = 0.0;
    currentSample_ = 0;
    
    // 5. 重置音频信息
    audioInfo_ = AudioInfo();
    
    LOG_DEBUG("音频解码器已关闭");
}

std::vector<AudioFrameData> AudioDecoder::getAllFrames() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    std::vector<AudioFrameData> frames;
    if (!opened_) {
        LOG_ERROR("音频文件未打开");
        return frames;
    }
    
    // 重置到文件开始
    if (!seekToTime(0.0)) {
        LOG_ERROR("无法定位到文件开始");
        return frames;
    }
    
    AudioFrameData frameData;
    int maxFrames = 50000; // 音频帧数通常很多，设置一个合理的上限
    int frameCount = 0;
    
    while (readNextFrame(frameData) && frameCount < maxFrames) {
        frames.push_back(frameData);
        frameCount++;
        
        // 每处理1000帧输出一次进度
        if (frameCount % 1000 == 0) {
            LOG_DEBUG("已处理 " + std::to_string(frameCount) + " 音频帧");
        }
    }
    
    LOG_INFO("音频解码完成，共 " + std::to_string(frames.size()) + " 帧");
    return frames;
}

bool AudioDecoder::readNextFrame(AudioFrameData& frameData) {
    if (!opened_) return false;
    
    while (av_read_frame(formatCtx_, packet_) >= 0) {
        if (packet_->stream_index == audioStreamIndex_) {
            if (processPacket(frameData)) {
                av_packet_unref(packet_);
                return true;
            }
        }
        av_packet_unref(packet_);
    }
    
    return false;
}

bool AudioDecoder::seekToTime(double seconds) {
    if (!opened_) return false;
    
    AVStream* stream = formatCtx_->streams[audioStreamIndex_];
    int64_t timestamp = av_rescale_q(seconds * AV_TIME_BASE, AV_TIME_BASE_Q, stream->time_base);
    
    if (av_seek_frame(formatCtx_, audioStreamIndex_, timestamp, AVSEEK_FLAG_BACKWARD) < 0) {
        return false;
    }
    
    avcodec_flush_buffers(codecCtx_);
    currentTime_ = seconds;
    return true;
}

bool AudioDecoder::seekToSample(int64_t sampleNumber) {
    if (!opened_) return false;
    
    double seconds = (double)sampleNumber / audioInfo_.sampleRate;
    return seekToTime(seconds);
}

bool AudioDecoder::initDecoder() {
    AVStream* stream = formatCtx_->streams[audioStreamIndex_];
    const AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!codec) {
        LOG_ERROR("找不到音频解码器");
        return false;
    }
    
    codecCtx_ = avcodec_alloc_context3(codec);
    if (!codecCtx_) {
        LOG_ERROR("无法分配音频解码器上下文");
        return false;
    }
    
    if (avcodec_parameters_to_context(codecCtx_, stream->codecpar) < 0) {
        LOG_ERROR("无法复制音频解码器参数");
        avcodec_free_context(&codecCtx_);
        return false;
    }
    
    if (avcodec_open2(codecCtx_, codec, nullptr) < 0) {
        LOG_ERROR("无法打开音频解码器");
        avcodec_free_context(&codecCtx_);
        return false;
    }
    
    return initSwrContext();
}

bool AudioDecoder::initSwrContext() {
    if (swrCtx_) {
        swr_free(&swrCtx_);
    }
    
    // 设置重采样参数
    swrCtx_ = swr_alloc_set_opts(nullptr,
        av_get_default_channel_layout(config_.outputChannels), // 输出声道布局
        config_.outputFormat,                                  // 输出格式
        config_.outputSampleRate,                             // 输出采样率
        av_get_default_channel_layout(codecCtx_->channels),   // 输入声道布局
        codecCtx_->sample_fmt,                                // 输入格式
        codecCtx_->sample_rate,                               // 输入采样率
        0, nullptr);
    
    if (!swrCtx_) {
        LOG_ERROR("无法创建重采样上下文");
        return false;
    }
    
    if (swr_init(swrCtx_) < 0) {
        LOG_ERROR("无法初始化重采样上下文");
        swr_free(&swrCtx_);
        return false;
    }
    
    return true;
}

void AudioDecoder::cleanup() {
    LOG_DEBUG("开始清理AudioDecoder资源...");
    
    if (packet_) {
        av_packet_free(&packet_);
        packet_ = nullptr;
        LOG_DEBUG("Audio AVPacket已释放");
    }
    
    if (frame_) {
        av_frame_free(&frame_);
        frame_ = nullptr;
        LOG_DEBUG("Audio AVFrame已释放");
    }
    
    LOG_DEBUG("AudioDecoder资源清理完成");
}

bool AudioDecoder::processPacket(AudioFrameData& frameData) {
    if (avcodec_send_packet(codecCtx_, packet_) != 0) {
        return false;
    }
    
    if (avcodec_receive_frame(codecCtx_, frame_) != 0) {
        return false;
    }
    
    if (!swrCtx_) {
        if (!initSwrContext()) {
            return false;
        }
    }
    
    AVStream* stream = formatCtx_->streams[audioStreamIndex_];
    
    // 计算输出样本数
    int outputSamples = swr_get_out_samples(swrCtx_, frame_->nb_samples);
    int outputSize = outputSamples * config_.outputChannels * av_get_bytes_per_sample(config_.outputFormat);
    
    // 分配输出缓冲区
    frameData.data.resize(outputSize);
    uint8_t* outputBuffer = frameData.data.data();
    
    // 重采样
    int convertedSamples = swr_convert(swrCtx_, &outputBuffer, outputSamples,
                                      (const uint8_t**)frame_->data, frame_->nb_samples);
    
    if (convertedSamples < 0) {
        LOG_ERROR("音频重采样失败");
        return false;
    }
    
    // 调整数据大小
    int actualSize = convertedSamples * config_.outputChannels * av_get_bytes_per_sample(config_.outputFormat);
    frameData.data.resize(actualSize);
    
    // 填充AudioFrameData
    frameData.sampleRate = config_.outputSampleRate;
    frameData.channels = config_.outputChannels;
    frameData.nbSamples = convertedSamples;
    frameData.duration = (double)convertedSamples / config_.outputSampleRate;
    frameData.pts = frame_->pts;
    frameData.timestamp = frame_->pts * av_q2d(stream->time_base);
    frameData.format = av_get_sample_fmt_name(config_.outputFormat);
    frameData.encoded = false;
    
    currentTime_ = frameData.timestamp;
    currentSample_ += convertedSamples;
    
    return true;
} 