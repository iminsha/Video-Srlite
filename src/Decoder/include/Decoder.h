#pragma once

#include <string>
#include <vector>
#include "VideoDecoder.h"
#include "AudioDecoder.h"
#include "../../DataStruct/FrameData.h"
#include "../../DataStruct/AudioFrameData.h"
#include <memory>

// 统一解码器配置
struct DecoderConfig {
    VideoDecoderConfig videoConfig;
    AudioDecoderConfig audioConfig;
    bool enableVideo = true;
    bool enableAudio = true;
    bool threadSafe = false;
};

// 媒体信息
struct MediaInfo {
    VideoInfo videoInfo;
    AudioInfo audioInfo;
    bool hasVideo = false;
    bool hasAudio = false;
    double duration = 0.0;
    std::string filename;
    int64_t fileSize = 0;
};

class Decoder {
public:
    Decoder();
    ~Decoder();
    
    // 初始化和配置
    bool initialize(const DecoderConfig& config = DecoderConfig());
    void destroy();
    bool isInitialized() const { return initialized_; }
    
    // 文件操作
    bool open(const std::string& filepath);
    void close();
    bool isOpened() const { return opened_; }
    
    // 获取信息
    MediaInfo getMediaInfo() const { return mediaInfo_; }
    
    // 解码操作
    std::vector<FrameData> getAllVideoFrames();
    std::vector<AudioFrameData> getAllAudioFrames();
    bool getAllFrames(std::vector<FrameData>& videoFrames, 
                      std::vector<AudioFrameData>& audioFrames);
    
    // 单帧解码
    bool readNextVideoFrame(FrameData& frame);
    bool readNextAudioFrame(AudioFrameData& frame);
    
    // 定位操作
    bool seekToTime(double seconds);
    bool seekToVideoFrame(int64_t frameNumber);
    bool seekToAudioSample(int64_t sampleNumber);
    
    // 配置和控制
    void setConfig(const DecoderConfig& config);
    DecoderConfig getConfig() const { return config_; }
    void setThreadSafe(bool enable);
    
    // 状态查询
    bool hasVideo() const { return mediaInfo_.hasVideo; }
    bool hasAudio() const { return mediaInfo_.hasAudio; }
    double getCurrentTime() const;
    
    // 获取解码器实例
    VideoDecoder* getVideoDecoder() { return videoDecoder_.get(); }
    AudioDecoder* getAudioDecoder() { return audioDecoder_.get(); }
    
private:
    // 内部方法
    bool initDecoders();
    void cleanup();
    bool analyzeFile(const std::string& filepath);
    
    // 解码器实例
    std::unique_ptr<VideoDecoder> videoDecoder_;
    std::unique_ptr<AudioDecoder> audioDecoder_;
    
    // 配置和状态
    DecoderConfig config_;
    MediaInfo mediaInfo_;
    bool initialized_ = false;
    bool opened_ = false;
    std::string currentFile_;
}; 