#include "VideoEncoder.h"
#include "../Utils/Logger.h"
#include <opencv2/opencv.hpp>
#include <chrono>

VideoEncoder::VideoEncoder()
    : codec_(nullptr)
    , codecContext_(nullptr)
    , frame_(nullptr)
    , swsContext_(nullptr)
    , lastSrcWidth_(0)
    , lastSrcHeight_(0)
    , initialized_(false)
    , frameIndex_(0) {
    LOG_DEBUG("VideoEncoder created");
}

VideoEncoder::~VideoEncoder() {
    close();
    LOG_DEBUG("VideoEncoder destroyed");
}

bool VideoEncoder::init(const VideoEncoderConfig& config, AVRational timeBase) {
    if (initialized_) {
        LOG_ERROR("VideoEncoder already initialized");
        return false;
    }
    
    config_ = config;
    timeBase_ = timeBase;
    
    LOG_INFO("Initializing video encoder: " + config_.codec);
    LOG_INFO("Resolution: " + std::to_string(config_.width) + "x" + std::to_string(config_.height));
    LOG_INFO("Bitrate: " + std::to_string(config_.bitrate/1000) + "kbps");
    LOG_INFO("Frame rate: " + std::to_string(config_.frameRate) + "fps");
    
    if (!setupCodec()) {
        LOG_ERROR("Failed to setup codec");
        return false;
    }
    
    initialized_ = true;
    frameIndex_ = 0;
    
    LOG_INFO("VideoEncoder initialized successfully");
    return true;
}

bool VideoEncoder::encode(const FrameData& frameData, std::vector<AVPacket*>& packets) {
    if (!initialized_) {
        LOG_ERROR("VideoEncoder not initialized");
        return false;
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    // 转换帧数据
    AVFrame* avFrame = convertFrameData(frameData);
    if (!avFrame) {
        LOG_ERROR("Failed to convert frame data");
        return false;
    }
    
    // 时间戳已经在convertFrameData中设置了，不要重复设置
    
    // 发送帧到编码器
    int ret = avcodec_send_frame(codecContext_, avFrame);
    if (ret < 0) {
        LOG_ERROR("Error sending frame to encoder: " + std::to_string(ret));
        return false;
    }
    
    // 接收编码包
    while (ret >= 0) {
        AVPacket* packet = av_packet_alloc();
        if (!packet) {
            LOG_ERROR("Failed to allocate packet");
            return false;
        }
        
        ret = avcodec_receive_packet(codecContext_, packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            av_packet_free(&packet);
            break;
        } else if (ret < 0) {
            LOG_ERROR("Error receiving packet from encoder: " + std::to_string(ret));
            av_packet_free(&packet);
            return false;
        }
        
        packets.push_back(packet);
        
        // 更新统计信息
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        updateStatistics(duration.count() / 1000.0, packet->size);
    }
    
    // 注意：不要释放avFrame，因为它指向的是frame_成员变量
    return true;
}

bool VideoEncoder::flush(std::vector<AVPacket*>& packets) {
    if (!initialized_) {
        LOG_ERROR("VideoEncoder not initialized");
        return false;
    }
    
    LOG_DEBUG("Flushing video encoder");
    
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
        updateStatistics(0.0, packet->size);
    }
    
    LOG_DEBUG("Video encoder flushed, " + std::to_string(packets.size()) + " packets");
    return true;
}

void VideoEncoder::close() {
    if (swsContext_) {
        sws_freeContext(swsContext_);
        swsContext_ = nullptr;
    }
    
    if (frame_) {
        av_frame_free(&frame_);
    }
    
    if (codecContext_) {
        avcodec_free_context(&codecContext_);
    }
    
    initialized_ = false;
    LOG_DEBUG("VideoEncoder closed");
}

VideoEncoder::Statistics VideoEncoder::getStatistics() const {
    return stats_;
}

bool VideoEncoder::setupCodec() {
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
    codecContext_->width = config_.width;
    codecContext_->height = config_.height;
    codecContext_->time_base = timeBase_;
    codecContext_->framerate = av_d2q(config_.frameRate, 1000);
    codecContext_->pix_fmt = config_.pixelFormat;
    codecContext_->gop_size = config_.gopSize;
    codecContext_->max_b_frames = 1;
    
    // CRF模式或码率模式
    if (config_.crf >= 0) {
        // CRF模式 - 恒定质量
        codecContext_->bit_rate = 0;  // 不设置码率
        codecContext_->flags |= AV_CODEC_FLAG_QSCALE;
        codecContext_->global_quality = FF_QP2LAMBDA * config_.crf;
    } else {
        // 码率模式
        codecContext_->bit_rate = config_.bitrate;
    }
    
    // 设置线程数
    if (config_.threadCount > 0) {
        codecContext_->thread_count = config_.threadCount;
    }
    
    // 设置编码器特定选项（x264）
    if (config_.codec == "libx264") {
        av_opt_set(codecContext_->priv_data, "preset", config_.preset.c_str(), 0);
        av_opt_set(codecContext_->priv_data, "profile", config_.profile.c_str(), 0);
        
        // CRF模式的特殊设置
        if (config_.crf >= 0) {
            av_opt_set(codecContext_->priv_data, "crf", std::to_string(config_.crf).c_str(), 0);
            if (config_.crf == 0) {
                // 无损压缩
                av_opt_set(codecContext_->priv_data, "qp", "0", 0);
            }
        } else {
            av_opt_set(codecContext_->priv_data, "tune", "zerolatency", 0);
        }
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
    
    frame_->format = codecContext_->pix_fmt;
    frame_->width = codecContext_->width;
    frame_->height = codecContext_->height;
    
    ret = av_frame_get_buffer(frame_, 0);
    if (ret < 0) {
        LOG_ERROR("Failed to allocate frame buffer: " + std::to_string(ret));
        return false;
    }
    
    return true;
}

AVFrame* VideoEncoder::convertFrameData(const FrameData& frameData) {
    if (frameData.image.empty()) {
        LOG_ERROR("Input image is empty");
        return nullptr;
    }
    
    // 重新分配frame以匹配当前尺寸
    if (frame_->width != frameData.width || frame_->height != frameData.height) {
        av_frame_unref(frame_);
        frame_->format = codecContext_->pix_fmt;
        frame_->width = frameData.width;
        frame_->height = frameData.height;
        
        int ret = av_frame_get_buffer(frame_, 0);
        if (ret < 0) {
            LOG_ERROR("Failed to reallocate frame buffer: " + std::to_string(ret));
            return nullptr;
        }
    }
    
    // 确保帧可写
    if (av_frame_make_writable(frame_) < 0) {
        LOG_ERROR("Failed to make frame writable");
        return nullptr;
    }
    
    // 使用SwScale进行转换
    if (!setupSwsContext(frameData.width, frameData.height)) {
        LOG_ERROR("Failed to setup SwsContext");
        return nullptr;
    }
    
    // 创建输入数据指针和行大小
    const uint8_t* srcData[4] = {0};
    int srcLinesize[4] = {0};
    
    // OpenCV Mat是连续的BGR数据
    srcData[0] = frameData.image.data;
    srcLinesize[0] = frameData.image.step[0];  // 使用OpenCV的step
    
    // 进行格式转换
    int ret = sws_scale(swsContext_,
                       srcData, srcLinesize, 0, frameData.height,
                       frame_->data, frame_->linesize);
    
    if (ret < 0) {
        LOG_ERROR("Failed to scale frame: " + std::to_string(ret));
        return nullptr;
    }
    
    // 设置时间戳 - 基于帧索引和时间基准
    frame_->pts = frameIndex_++;
    
    LOG_DEBUG("Frame " + std::to_string(frameIndex_-1) + " PTS: " + std::to_string(frame_->pts));
    
    return frame_;
}

bool VideoEncoder::setupSwsContext(int srcWidth, int srcHeight) {
    // 如果尺寸变化了，重新创建SwsContext
    if (swsContext_ && (lastSrcWidth_ != srcWidth || lastSrcHeight_ != srcHeight)) {
        sws_freeContext(swsContext_);
        swsContext_ = nullptr;
    }
    
    if (!swsContext_) {
        // 输入是BGR24格式，输出是编码器要求的格式
        swsContext_ = sws_getContext(
            srcWidth, srcHeight, AV_PIX_FMT_BGR24,
            frame_->width, frame_->height, codecContext_->pix_fmt,
            SWS_BICUBIC, nullptr, nullptr, nullptr);
        
        if (!swsContext_) {
            LOG_ERROR("Failed to create SwsContext");
            return false;
        }
        
        lastSrcWidth_ = srcWidth;
        lastSrcHeight_ = srcHeight;
    }
    
    return true;
}

void VideoEncoder::updateStatistics(double encodingTime, size_t packetSize) {
    stats_.framesEncoded++;
    stats_.bytesEncoded += packetSize;
    
    // 更新平均编码时间
    if (stats_.framesEncoded > 0) {
        stats_.avgEncodingTime = (stats_.avgEncodingTime * (stats_.framesEncoded - 1) + encodingTime) / stats_.framesEncoded;
    }
}

AVPixelFormat VideoEncoder::getAVPixelFormat(const std::string& format) const {
    if (format == "bgr24") return AV_PIX_FMT_BGR24;
    if (format == "rgb24") return AV_PIX_FMT_RGB24;
    if (format == "yuv420p") return AV_PIX_FMT_YUV420P;
    if (format == "nv12") return AV_PIX_FMT_NV12;
    
    // 默认返回BGR24
    return AV_PIX_FMT_BGR24;
}

bool VideoEncoder::needsConversion(const FrameData& frameData) const {
    // 检查尺寸
    if (frameData.width != config_.width || frameData.height != config_.height) {
        return true;
    }
    
    // 检查像素格式
    AVPixelFormat inputFormat = getAVPixelFormat(frameData.pixFormat);
    if (inputFormat != config_.pixelFormat) {
        return true;
    }
    
    return false;
}
