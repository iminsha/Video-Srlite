#include "../include/VideoDecoder.h"
#include "../../Utils/Logger.h"
#include <cstring>

VideoDecoder::VideoDecoder() {
    // FFmpeg初始化在第一次使用时进行
}

VideoDecoder::~VideoDecoder() {
    destroy();
}

bool VideoDecoder::initialize(const VideoDecoderConfig& config) {
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
    LOG_DEBUG("VideoDecoder初始化成功");
    return true;
}

void VideoDecoder::destroy() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    if (!initialized_) return;
    
    LOG_DEBUG("开始销毁视频解码器...");
    
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
    
    LOG_DEBUG("VideoDecoder已销毁");
}

bool VideoDecoder::open(const std::string& filepath) {
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
        LOG_ERROR("无法打开视频文件: " + filepath);
        return false;
    }
    
    // 查找流信息
    if (avformat_find_stream_info(formatCtx_, nullptr) < 0) {
        LOG_ERROR("无法找到流信息");
        avformat_close_input(&formatCtx_);
        return false;
    }
    
    // 查找视频流
    videoStreamIndex_ = -1;
    for (unsigned int i = 0; i < formatCtx_->nb_streams; i++) {
        if (formatCtx_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex_ = i;
            break;
        }
    }
    
    if (videoStreamIndex_ == -1) {
        LOG_WARNING("未找到视频流");
        avformat_close_input(&formatCtx_);
        return false;
    }
    
    if (!initDecoder()) {
        avformat_close_input(&formatCtx_);
        return false;
    }
    
    // 填充视频信息
    AVStream* stream = formatCtx_->streams[videoStreamIndex_];
    videoInfo_.width = codecCtx_->width;
    videoInfo_.height = codecCtx_->height;
    videoInfo_.frameRate = av_q2d(stream->avg_frame_rate);
    videoInfo_.duration = formatCtx_->duration / (double)AV_TIME_BASE;
    videoInfo_.totalFrames = stream->nb_frames;
    videoInfo_.codecName = avcodec_get_name(codecCtx_->codec_id);
    videoInfo_.pixelFormat = av_get_pix_fmt_name(codecCtx_->pix_fmt) ? av_get_pix_fmt_name(codecCtx_->pix_fmt) : "unknown";
    
    opened_ = true;
    currentTime_ = 0.0;
    currentFrame_ = 0;
    
    LOG_INFO("视频解码器打开成功: " + std::to_string(videoInfo_.width) + "x" + 
             std::to_string(videoInfo_.height) + " @" + 
             std::to_string(videoInfo_.frameRate) + "fps");
    
    return true;
}

void VideoDecoder::close() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    if (!opened_) return;
    
    LOG_DEBUG("开始关闭视频解码器...");
    
    // 1. 首先关闭SwsContext
    if (swsCtx_) {
        sws_freeContext(swsCtx_);
        swsCtx_ = nullptr;
        LOG_DEBUG("SwsContext已释放");
    }
    
    // 2. 关闭编解码器上下文
    if (codecCtx_) {
        avcodec_free_context(&codecCtx_);
        codecCtx_ = nullptr;
        LOG_DEBUG("CodecContext已释放");
    }
    
    // 3. 最后关闭格式上下文
    if (formatCtx_) {
        avformat_close_input(&formatCtx_);
        formatCtx_ = nullptr;
        LOG_DEBUG("FormatContext已释放");
    }
    
    // 4. 重置状态
    videoStreamIndex_ = -1;
    opened_ = false;
    currentTime_ = 0.0;
    currentFrame_ = 0;
    
    // 5. 重置视频信息
    videoInfo_ = VideoInfo();
    
    LOG_DEBUG("视频解码器已关闭");
}

std::vector<FrameData> VideoDecoder::getAllFrames() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    std::vector<FrameData> frames;
    if (!opened_) {
        LOG_ERROR("视频文件未打开");
        return frames;
    }
    
    // 重置到文件开始
    if (!seekToTime(0.0)) {
        LOG_ERROR("无法定位到文件开始");
        return frames;
    }
    
    FrameData frameData;
    int maxFrames = videoInfo_.totalFrames > 0 ? videoInfo_.totalFrames + 10 : 10000; // 安全上限
    int frameCount = 0;
    
    while (readNextFrame(frameData) && frameCount < maxFrames) {
        frames.push_back(frameData);
        frameCount++;
        
        // 每处理100帧输出一次进度
        if (frameCount % 100 == 0) {
            LOG_DEBUG("已处理 " + std::to_string(frameCount) + " 帧");
        }
    }
    
    LOG_INFO("视频解码完成，共 " + std::to_string(frames.size()) + " 帧");
    return frames;
}

bool VideoDecoder::readNextFrame(FrameData& frameData) {
    if (!opened_) return false;
    
    while (av_read_frame(formatCtx_, packet_) >= 0) {
        if (packet_->stream_index == videoStreamIndex_) {
            if (processPacket(frameData)) {
                av_packet_unref(packet_);
                return true;
            }
        }
        av_packet_unref(packet_);
    }
    
    return false;
}

bool VideoDecoder::seekToTime(double seconds) {
    if (!opened_) return false;
    
    AVStream* stream = formatCtx_->streams[videoStreamIndex_];
    int64_t timestamp = av_rescale_q(seconds * AV_TIME_BASE, AV_TIME_BASE_Q, stream->time_base);
    
    if (av_seek_frame(formatCtx_, videoStreamIndex_, timestamp, AVSEEK_FLAG_BACKWARD) < 0) {
        return false;
    }
    
    avcodec_flush_buffers(codecCtx_);
    currentTime_ = seconds;
    return true;
}

bool VideoDecoder::seekToFrame(int64_t frameNumber) {
    if (!opened_) return false;
    
    double seconds = frameNumber / videoInfo_.frameRate;
    return seekToTime(seconds);
}

bool VideoDecoder::initDecoder() {
    AVStream* stream = formatCtx_->streams[videoStreamIndex_];
    const AVCodec* codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!codec) {
        LOG_ERROR("找不到视频解码器");
        return false;
    }
    
    codecCtx_ = avcodec_alloc_context3(codec);
    if (!codecCtx_) {
        LOG_ERROR("无法分配解码器上下文");
        return false;
    }
    
    if (avcodec_parameters_to_context(codecCtx_, stream->codecpar) < 0) {
        LOG_ERROR("无法复制解码器参数");
        avcodec_free_context(&codecCtx_);
        return false;
    }
    
    // 设置线程数
    if (config_.threadCount > 0) {
        codecCtx_->thread_count = config_.threadCount;
    }
    
    if (avcodec_open2(codecCtx_, codec, nullptr) < 0) {
        LOG_ERROR("无法打开解码器");
        avcodec_free_context(&codecCtx_);
        return false;
    }
    
    return initSwsContext();
}

bool VideoDecoder::initSwsContext() {
    if (swsCtx_) {
        sws_freeContext(swsCtx_);
    }
    
    AVPixelFormat outputFormat = AV_PIX_FMT_BGR24;
    if (config_.outputPixelFormat == "rgb24") {
        outputFormat = AV_PIX_FMT_RGB24;
    } else if (config_.outputPixelFormat == "gray") {
        outputFormat = AV_PIX_FMT_GRAY8;
    }
    
    int outputWidth = codecCtx_->width;
    int outputHeight = codecCtx_->height;
    
    // 应用尺寸限制
    if (config_.maxWidth > 0 && outputWidth > config_.maxWidth) {
        outputHeight = outputHeight * config_.maxWidth / outputWidth;
        outputWidth = config_.maxWidth;
    }
    if (config_.maxHeight > 0 && outputHeight > config_.maxHeight) {
        outputWidth = outputWidth * config_.maxHeight / outputHeight;
        outputHeight = config_.maxHeight;
    }
    
    swsCtx_ = sws_getContext(
        codecCtx_->width, codecCtx_->height, codecCtx_->pix_fmt,
        outputWidth, outputHeight, outputFormat,
        SWS_BILINEAR, nullptr, nullptr, nullptr
    );
    
    if (!swsCtx_) {
        LOG_ERROR("无法创建图像转换上下文");
        return false;
    }
    
    return true;
}

void VideoDecoder::cleanup() {
    LOG_DEBUG("开始清理VideoDecoder资源...");
    
    if (packet_) {
        av_packet_free(&packet_);
        packet_ = nullptr;
        LOG_DEBUG("AVPacket已释放");
    }
    
    if (frame_) {
        av_frame_free(&frame_);
        frame_ = nullptr;
        LOG_DEBUG("AVFrame已释放");
    }
    
    LOG_DEBUG("VideoDecoder资源清理完成");
}

bool VideoDecoder::processPacket(FrameData& frameData) {
    if (avcodec_send_packet(codecCtx_, packet_) != 0) {
        return false;
    }
    
    if (avcodec_receive_frame(codecCtx_, frame_) != 0) {
        return false;
    }
    
    if (!swsCtx_) {
        if (!initSwsContext()) {
            return false;
        }
    }
    
    AVStream* stream = formatCtx_->streams[videoStreamIndex_];
    
    // 填充FrameData
    frameData.width = frame_->width;
    frameData.height = frame_->height;
    frameData.pts = frame_->pts;
    frameData.timestamp = frame_->pts * av_q2d(stream->time_base);
    frameData.frameIndex = currentFrame_++;
    frameData.sourceTag = "video";
    
    // 创建OpenCV Mat
    int channels = 3;
    if (config_.outputPixelFormat == "gray") {
        channels = 1;
    }
    
    frameData.image = cv::Mat(frame_->height, frame_->width, 
                             channels == 1 ? CV_8UC1 : CV_8UC3);
    
    uint8_t* dstData[4] = { frameData.image.data, nullptr, nullptr, nullptr };
    int dstLinesize[4] = { static_cast<int>(frameData.image.step), 0, 0, 0 };
    
    sws_scale(swsCtx_, frame_->data, frame_->linesize, 0, frame_->height,
             dstData, dstLinesize);
    
    currentTime_ = frameData.timestamp;
    return true;
} 