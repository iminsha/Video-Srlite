#pragma once

#include <string>
#include <vector>
#include <mutex>
#include "../../DataStruct/FrameData.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/pixdesc.h>
}

// 视频解码器配置
struct VideoDecoderConfig {
    std::string outputPixelFormat = "bgr24";  // 输出像素格式
    int maxWidth = 0;                         // 最大宽度（0表示不限制）
    int maxHeight = 0;                        // 最大高度（0表示不限制）
    bool enableHardwareAccel = false;         // 是否启用硬件加速
    int threadCount = 0;                      // 解码线程数（0表示自动）
};

// 视频信息
struct VideoInfo {
    int width = 0;
    int height = 0;
    double frameRate = 0.0;
    double duration = 0.0;
    int64_t totalFrames = 0;
    std::string codecName;
    std::string pixelFormat;
};

class VideoDecoder {
public:
    VideoDecoder();
    ~VideoDecoder();
    
    // 初始化和配置
    bool initialize(const VideoDecoderConfig& config = VideoDecoderConfig());
    void destroy();
    bool isInitialized() const { return initialized_; }
    
    // 文件操作
    bool open(const std::string& filepath);
    void close();
    bool isOpened() const { return opened_; }
    
    // 获取信息
    VideoInfo getVideoInfo() const { return videoInfo_; }
    
    // 解码操作
    std::vector<FrameData> getAllFrames();
    bool readNextFrame(FrameData& frame);
    bool seekToTime(double seconds);
    bool seekToFrame(int64_t frameNumber);
    
    // 配置和控制
    void setThreadSafe(bool enable) { threadSafe_ = enable; }
    void setConfig(const VideoDecoderConfig& config) { config_ = config; }
    VideoDecoderConfig getConfig() const { return config_; }
    
    // 状态查询
    bool hasVideo() const { return videoStreamIndex_ != -1 && opened_; }
    double getCurrentTime() const { return currentTime_; }
    int64_t getCurrentFrame() const { return currentFrame_; }
    
private:
    // 内部方法
    bool initDecoder();
    bool initSwsContext();
    void cleanup();
    bool processPacket(FrameData& frame);
    
    // FFmpeg组件
    AVFormatContext* formatCtx_ = nullptr;
    AVCodecContext* codecCtx_ = nullptr;
    AVPacket* packet_ = nullptr;
    AVFrame* frame_ = nullptr;
    SwsContext* swsCtx_ = nullptr;
    
    // 配置和状态
    VideoDecoderConfig config_;
    VideoInfo videoInfo_;
    bool initialized_ = false;
    bool opened_ = false;
    int videoStreamIndex_ = -1;
    double currentTime_ = 0.0;
    int64_t currentFrame_ = 0;
    
    // 线程安全
    bool threadSafe_ = false;
    mutable std::mutex mutex_;
}; 