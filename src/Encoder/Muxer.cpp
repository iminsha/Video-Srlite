#include "Muxer.h"
#include "../Utils/Logger.h"

Muxer::Muxer()
    : formatContext_(nullptr)
    , outputFormat_(nullptr)
    , videoStreamIndex_(-1)
    , audioStreamIndex_(-1)
    , initialized_(false)
    , headerWritten_(false)
    , finalized_(false) {
    LOG_DEBUG("Muxer created");
}

Muxer::~Muxer() {
    if (!finalized_) {
        finalize();
    }
    cleanup();
    LOG_DEBUG("Muxer destroyed");
}

bool Muxer::init(const MuxerConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (initialized_) {
        LOG_ERROR("Muxer already initialized");
        return false;
    }
    
    config_ = config;
    
    LOG_INFO("Initializing muxer for output: " + config_.outputPath);
    LOG_INFO("Format: " + config_.format);
    
    if (!setupOutputFormat()) {
        LOG_ERROR("Failed to setup output format");
        return false;
    }
    
    initialized_ = true;
    LOG_INFO("Muxer initialized successfully");
    return true;
}

int Muxer::addVideoStream(AVCodecContext* codecContext) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!initialized_ || !codecContext) {
        LOG_ERROR("Muxer not initialized or invalid codec context");
        return -1;
    }
    
    if (headerWritten_) {
        LOG_ERROR("Cannot add stream after header is written");
        return -1;
    }
    
    if (!setupOutputStream(codecContext, true)) {
        LOG_ERROR("Failed to setup video stream");
        return -1;
    }
    
    videoStreamIndex_ = streams_.size() - 1;
    LOG_INFO("Added video stream at index: " + std::to_string(videoStreamIndex_));
    return videoStreamIndex_;
}

int Muxer::addAudioStream(AVCodecContext* codecContext) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!initialized_ || !codecContext) {
        LOG_ERROR("Muxer not initialized or invalid codec context");
        return -1;
    }
    
    if (headerWritten_) {
        LOG_ERROR("Cannot add stream after header is written");
        return -1;
    }
    
    if (!setupOutputStream(codecContext, false)) {
        LOG_ERROR("Failed to setup audio stream");
        return -1;
    }
    
    audioStreamIndex_ = streams_.size() - 1;
    LOG_INFO("Added audio stream at index: " + std::to_string(audioStreamIndex_));
    return audioStreamIndex_;
}

bool Muxer::writeHeader() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!initialized_) {
        LOG_ERROR("Muxer not initialized");
        return false;
    }
    
    if (headerWritten_) {
        LOG_WARNING("Header already written");
        return true;
    }
    
    if (streams_.empty()) {
        LOG_ERROR("No streams added");
        return false;
    }
    
    LOG_INFO("Writing file header...");
    
    // 设置MP4快速启动
    if (config_.fastStart && config_.format == "mp4") {
        av_dict_set(&formatContext_->metadata, "movflags", "faststart", 0);
    }
    
    // 写入文件头
    int ret = avformat_write_header(formatContext_, nullptr);
    if (ret < 0) {
        LOG_ERROR("Failed to write header: " + std::to_string(ret));
        return false;
    }
    
    headerWritten_ = true;
    stats_.headerWritten = true;
    
    LOG_INFO("File header written successfully");
    return true;
}

bool Muxer::writePacket(AVPacket* packet, int streamIndex) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!initialized_ || !headerWritten_) {
        LOG_ERROR("Muxer not ready for writing packets");
        return false;
    }
    
    if (finalized_) {
        LOG_ERROR("Muxer already finalized");
        return false;
    }
    
    if (!validatePacket(packet, streamIndex)) {
        LOG_ERROR("Invalid packet or stream index");
        return false;
    }
    
    // 重新调整时间戳
    rescalePacketTimestamps(packet, streamIndex);
    
    // 写入数据包
    int ret;
    if (config_.enableInterleaving) {
        ret = av_interleaved_write_frame(formatContext_, packet);
    } else {
        ret = av_write_frame(formatContext_, packet);
    }
    
    if (ret < 0) {
        LOG_ERROR("Failed to write packet: " + std::to_string(ret));
        return false;
    }
    
    // 更新统计信息
    bool isVideo = (streamIndex == videoStreamIndex_);
    updateStatistics(packet, isVideo);
    
    return true;
}

bool Muxer::finalize() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!initialized_ || finalized_) {
        return true;
    }
    
    if (!headerWritten_) {
        LOG_WARNING("Finalizing without writing header");
        return true;
    }
    
    LOG_INFO("Finalizing muxer...");
    
    // 写入文件尾
    int ret = av_write_trailer(formatContext_);
    if (ret < 0) {
        LOG_ERROR("Failed to write trailer: " + std::to_string(ret));
        return false;
    }
    
    finalized_ = true;
    stats_.finalized = true;
    
    // 输出最终统计信息
    LOG_INFO("Muxing completed:");
    LOG_INFO("  Total packets: " + std::to_string(stats_.totalPackets));
    LOG_INFO("  Video packets: " + std::to_string(stats_.videoPackets));
    LOG_INFO("  Audio packets: " + std::to_string(stats_.audioPackets));
    LOG_INFO("  Total bytes: " + std::to_string(stats_.totalBytes));
    LOG_INFO("  Duration: " + std::to_string(stats_.totalDuration) + "s");
    
    return true;
}

Muxer::Statistics Muxer::getStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return stats_;
}

std::vector<std::string> Muxer::getSupportedFormats() {
    std::vector<std::string> formats;
    
    const AVOutputFormat* format = nullptr;
    void* iter = nullptr;
    
    while ((format = av_muxer_iterate(&iter))) {
        if (format->name) {
            formats.push_back(format->name);
        }
    }
    
    std::sort(formats.begin(), formats.end());
    return formats;
}

bool Muxer::isFormatSupported(const std::string& format) {
    const AVOutputFormat* outputFormat = av_guess_format(format.c_str(), nullptr, nullptr);
    return outputFormat != nullptr;
}

bool Muxer::setupOutputFormat() {
    // 猜测输出格式
    outputFormat_ = av_guess_format(config_.format.c_str(), config_.outputPath.c_str(), nullptr);
    if (!outputFormat_) {
        LOG_ERROR("Could not determine output format for: " + config_.format);
        return false;
    }
    
    // 分配格式上下文
    int ret = avformat_alloc_output_context2(&formatContext_, const_cast<AVOutputFormat*>(outputFormat_), nullptr, config_.outputPath.c_str());
    if (ret < 0) {
        LOG_ERROR("Failed to allocate output context: " + std::to_string(ret));
        return false;
    }
    
    // 打开输出文件
    if (!(outputFormat_->flags & AVFMT_NOFILE)) {
        ret = avio_open(&formatContext_->pb, config_.outputPath.c_str(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            LOG_ERROR("Failed to open output file: " + config_.outputPath + ", error: " + std::to_string(ret));
            return false;
        }
    }
    
    return true;
}

bool Muxer::setupOutputStream(AVCodecContext* codecContext, bool isVideo) {
    // 创建流
    AVStream* stream = avformat_new_stream(formatContext_, nullptr);
    if (!stream) {
        LOG_ERROR("Failed to create new stream");
        return false;
    }
    
    // 拷贝编码器参数
    int ret = avcodec_parameters_from_context(stream->codecpar, codecContext);
    if (ret < 0) {
        LOG_ERROR("Failed to copy codec parameters: " + std::to_string(ret));
        return false;
    }
    
    // 设置时间基准
    stream->time_base = codecContext->time_base;
    
    // 创建流信息
    StreamInfo streamInfo;
    streamInfo.index = stream->index;
    streamInfo.stream = stream;
    streamInfo.codecContext = codecContext;
    streamInfo.isVideo = isVideo;
    
    streams_.push_back(streamInfo);
    
    return true;
}

bool Muxer::validatePacket(AVPacket* packet, int streamIndex) const {
    if (!packet) {
        LOG_ERROR("Packet is null");
        return false;
    }
    
    if (streamIndex < 0 || streamIndex >= static_cast<int>(streams_.size())) {
        LOG_ERROR("Invalid stream index: " + std::to_string(streamIndex));
        return false;
    }
    
    if (packet->stream_index != streamIndex) {
        // 修正流索引
        packet->stream_index = streamIndex;
    }
    
    return true;
}

void Muxer::updateStatistics(AVPacket* packet, bool isVideo) {
    stats_.totalPackets++;
    stats_.totalBytes += packet->size;
    
    if (isVideo) {
        stats_.videoPackets++;
    } else {
        stats_.audioPackets++;
    }
    
    // 更新总时长
    if (packet->pts != AV_NOPTS_VALUE) {
        const StreamInfo& streamInfo = streams_[packet->stream_index];
        double timestamp = packet->pts * av_q2d(streamInfo.stream->time_base);
        if (timestamp > stats_.totalDuration) {
            stats_.totalDuration = timestamp;
        }
    }
}

void Muxer::rescalePacketTimestamps(AVPacket* packet, int streamIndex) {
    const StreamInfo& streamInfo = streams_[streamIndex];
    AVStream* stream = streamInfo.stream;
    
    // 重新调整时间戳到流的时间基准
    if (packet->pts != AV_NOPTS_VALUE) {
        packet->pts = av_rescale_q(packet->pts, streamInfo.codecContext->time_base, stream->time_base);
    }
    
    if (packet->dts != AV_NOPTS_VALUE) {
        packet->dts = av_rescale_q(packet->dts, streamInfo.codecContext->time_base, stream->time_base);
    }
    
    if (packet->duration > 0) {
        packet->duration = av_rescale_q(packet->duration, streamInfo.codecContext->time_base, stream->time_base);
    }
    
    // 确保DTS <= PTS
    if (packet->dts != AV_NOPTS_VALUE && packet->pts != AV_NOPTS_VALUE && packet->dts > packet->pts) {
        packet->dts = packet->pts;
    }
}

int64_t Muxer::getNextPts(int streamIndex, int64_t currentPts) {
    StreamInfo& streamInfo = streams_[streamIndex];
    
    if (streamInfo.lastPts == AV_NOPTS_VALUE) {
        streamInfo.lastPts = currentPts;
        return currentPts;
    }
    
    // 确保PTS单调递增
    if (currentPts <= streamInfo.lastPts) {
        currentPts = streamInfo.lastPts + 1;
    }
    
    streamInfo.lastPts = currentPts;
    return currentPts;
}

int64_t Muxer::getNextDts(int streamIndex, int64_t currentDts) {
    StreamInfo& streamInfo = streams_[streamIndex];
    
    if (streamInfo.lastDts == AV_NOPTS_VALUE) {
        streamInfo.lastDts = currentDts;
        return currentDts;
    }
    
    // 确保DTS单调递增
    if (currentDts <= streamInfo.lastDts) {
        currentDts = streamInfo.lastDts + 1;
    }
    
    streamInfo.lastDts = currentDts;
    return currentDts;
}

void Muxer::cleanup() {
    if (formatContext_) {
        if (!(outputFormat_->flags & AVFMT_NOFILE)) {
            avio_closep(&formatContext_->pb);
        }
        avformat_free_context(formatContext_);
        formatContext_ = nullptr;
    }
    
    streams_.clear();
    videoStreamIndex_ = -1;
    audioStreamIndex_ = -1;
    initialized_ = false;
    headerWritten_ = false;
    finalized_ = false;
} 