#include "../include/Decoder.h"
#include "../../Utils/Logger.h"
#include <filesystem>

Decoder::Decoder() {
    // 构造函数中不进行初始化，等待显式调用
}

Decoder::~Decoder() {
    destroy();
}

bool Decoder::initialize(const DecoderConfig& config) {
    if (initialized_) {
        return true;
    }
    
    config_ = config;
    
    if (!initDecoders()) {
        return false;
    }
    
    initialized_ = true;
    LOG_DEBUG("Decoder初始化成功");
    return true;
}

void Decoder::destroy() {
    if (!initialized_) return;
    
    close();
    cleanup();
    initialized_ = false;
    LOG_DEBUG("Decoder已销毁");
}

bool Decoder::open(const std::string& filepath) {
    if (!initialized_) {
        if (!initialize()) {
            return false;
        }
    }
    
    if (opened_) {
        close();
    }
    
    if (!analyzeFile(filepath)) {
        return false;
    }
    
    currentFile_ = filepath;
    bool success = true;
    
    // 根据配置打开相应的解码器
    if (config_.enableVideo && mediaInfo_.hasVideo) {
        videoDecoder_->setThreadSafe(config_.threadSafe);
        videoDecoder_->setConfig(config_.videoConfig);
        if (!videoDecoder_->open(filepath)) {
            LOG_ERROR("视频解码器打开失败");
            success = false;
        } else {
            mediaInfo_.videoInfo = videoDecoder_->getVideoInfo();
        }
    }
    
    if (config_.enableAudio && mediaInfo_.hasAudio) {
        audioDecoder_->setThreadSafe(config_.threadSafe);
        audioDecoder_->setConfig(config_.audioConfig);
        if (!audioDecoder_->open(filepath)) {
            LOG_ERROR("音频解码器打开失败");
            success = false;
        } else {
            mediaInfo_.audioInfo = audioDecoder_->getAudioInfo();
        }
    }
    
    if (success) {
        opened_ = true;
        LOG_INFO("媒体文件打开成功: " + mediaInfo_.filename + 
                 " (视频:" + (mediaInfo_.hasVideo ? "有" : "无") + 
                 " 音频:" + (mediaInfo_.hasAudio ? "有" : "无") + ")");
    } else {
        close();
    }
    
    return success;
}

void Decoder::close() {
    if (!opened_) return;
    
    if (videoDecoder_) {
        videoDecoder_->close();
    }
    if (audioDecoder_) {
        audioDecoder_->close();
    }
    
    opened_ = false;
    currentFile_.clear();
    mediaInfo_ = MediaInfo();
    LOG_DEBUG("媒体文件已关闭");
}

std::vector<FrameData> Decoder::getAllVideoFrames() {
    if (!opened_ || !mediaInfo_.hasVideo || !videoDecoder_) {
        LOG_ERROR("视频解码器未准备好");
        return std::vector<FrameData>();
    }
    
    return videoDecoder_->getAllFrames();
}

std::vector<AudioFrameData> Decoder::getAllAudioFrames() {
    if (!opened_ || !mediaInfo_.hasAudio || !audioDecoder_) {
        LOG_ERROR("音频解码器未准备好");
        return std::vector<AudioFrameData>();
    }
    
    return audioDecoder_->getAllFrames();
}

bool Decoder::getAllFrames(std::vector<FrameData>& videoFrames, 
                          std::vector<AudioFrameData>& audioFrames) {
    bool success = true;
    
    if (mediaInfo_.hasVideo && videoDecoder_) {
        videoFrames = getAllVideoFrames();
        if (videoFrames.empty()) {
            success = false;
        }
    }
    
    if (mediaInfo_.hasAudio && audioDecoder_) {
        audioFrames = getAllAudioFrames();
        if (audioFrames.empty()) {
            success = false;
        }
    }
    
    return success;
}

bool Decoder::readNextVideoFrame(FrameData& frame) {
    if (!opened_ || !mediaInfo_.hasVideo || !videoDecoder_) {
        return false;
    }
    
    return videoDecoder_->readNextFrame(frame);
}

bool Decoder::readNextAudioFrame(AudioFrameData& frame) {
    if (!opened_ || !mediaInfo_.hasAudio || !audioDecoder_) {
        return false;
    }
    
    return audioDecoder_->readNextFrame(frame);
}

bool Decoder::seekToTime(double seconds) {
    bool success = true;
    
    if (mediaInfo_.hasVideo && videoDecoder_) {
        if (!videoDecoder_->seekToTime(seconds)) {
            success = false;
        }
    }
    
    if (mediaInfo_.hasAudio && audioDecoder_) {
        if (!audioDecoder_->seekToTime(seconds)) {
            success = false;
        }
    }
    
    return success;
}

bool Decoder::seekToVideoFrame(int64_t frameNumber) {
    if (!opened_ || !mediaInfo_.hasVideo || !videoDecoder_) {
        return false;
    }
    
    return videoDecoder_->seekToFrame(frameNumber);
}

bool Decoder::seekToAudioSample(int64_t sampleNumber) {
    if (!opened_ || !mediaInfo_.hasAudio || !audioDecoder_) {
        return false;
    }
    
    return audioDecoder_->seekToSample(sampleNumber);
}

void Decoder::setConfig(const DecoderConfig& config) {
    config_ = config;
    
    if (videoDecoder_) {
        videoDecoder_->setConfig(config_.videoConfig);
        videoDecoder_->setThreadSafe(config_.threadSafe);
    }
    
    if (audioDecoder_) {
        audioDecoder_->setConfig(config_.audioConfig);
        audioDecoder_->setThreadSafe(config_.threadSafe);
    }
}

void Decoder::setThreadSafe(bool enable) {
    config_.threadSafe = enable;
    
    if (videoDecoder_) {
        videoDecoder_->setThreadSafe(enable);
    }
    
    if (audioDecoder_) {
        audioDecoder_->setThreadSafe(enable);
    }
}

double Decoder::getCurrentTime() const {
    double videoTime = 0.0;
    double audioTime = 0.0;
    
    if (mediaInfo_.hasVideo && videoDecoder_) {
        videoTime = videoDecoder_->getCurrentTime();
    }
    
    if (mediaInfo_.hasAudio && audioDecoder_) {
        audioTime = audioDecoder_->getCurrentTime();
    }
    
    // 返回较大的时间值
    return std::max(videoTime, audioTime);
}

bool Decoder::initDecoders() {
    // 创建解码器实例
    videoDecoder_ = std::make_unique<VideoDecoder>();
    audioDecoder_ = std::make_unique<AudioDecoder>();
    
    if (!videoDecoder_ || !audioDecoder_) {
        LOG_ERROR("无法创建解码器实例");
        return false;
    }
    
    // 初始化解码器
    if (!videoDecoder_->initialize(config_.videoConfig)) {
        LOG_ERROR("视频解码器初始化失败");
        return false;
    }
    
    if (!audioDecoder_->initialize(config_.audioConfig)) {
        LOG_ERROR("音频解码器初始化失败");
        return false;
    }
    
    return true;
}

void Decoder::cleanup() {
    if (videoDecoder_) {
        videoDecoder_->destroy();
        videoDecoder_.reset();
    }
    
    if (audioDecoder_) {
        audioDecoder_->destroy();
        audioDecoder_.reset();
    }
}

bool Decoder::analyzeFile(const std::string& filepath) {
    // 检查文件是否存在
    if (!std::filesystem::exists(filepath)) {
        LOG_ERROR("文件不存在: " + filepath);
        return false;
    }
    
    // 获取文件信息
    mediaInfo_.filename = std::filesystem::path(filepath).filename().string();
    mediaInfo_.fileSize = std::filesystem::file_size(filepath);
    
    // 使用FFmpeg分析文件
    AVFormatContext* formatCtx = nullptr;
    if (avformat_open_input(&formatCtx, filepath.c_str(), nullptr, nullptr) < 0) {
        LOG_ERROR("无法打开文件进行分析: " + filepath);
        return false;
    }
    
    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
        LOG_ERROR("无法获取流信息");
        avformat_close_input(&formatCtx);
        return false;
    }
    
    // 分析流类型
    mediaInfo_.hasVideo = false;
    mediaInfo_.hasAudio = false;
    
    for (unsigned int i = 0; i < formatCtx->nb_streams; i++) {
        AVCodecParameters* codecpar = formatCtx->streams[i]->codecpar;
        
        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO && !mediaInfo_.hasVideo) {
            mediaInfo_.hasVideo = true;
        } else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO && !mediaInfo_.hasAudio) {
            mediaInfo_.hasAudio = true;
        }
    }
    
    // 获取总时长
    if (formatCtx->duration != AV_NOPTS_VALUE) {
        mediaInfo_.duration = formatCtx->duration / (double)AV_TIME_BASE;
    }
    
    avformat_close_input(&formatCtx);
    
    LOG_DEBUG("文件分析完成: " + mediaInfo_.filename + 
              " 时长:" + std::to_string(mediaInfo_.duration) + "s " +
              "视频:" + (mediaInfo_.hasVideo ? "有" : "无") + " " +
              "音频:" + (mediaInfo_.hasAudio ? "有" : "无"));
    
    return true;
}