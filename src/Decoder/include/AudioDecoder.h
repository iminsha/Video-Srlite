#pragma once

#include <string>
#include <vector>
#include <mutex>
#include "../../DataStruct/AudioFrameData.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
}

// 音频解码器配置
struct AudioDecoderConfig {
    int outputSampleRate = 44100;        // 输出采样率
    int outputChannels = 2;              // 输出声道数
    AVSampleFormat outputFormat = AV_SAMPLE_FMT_S16;  // 输出格式
    int maxSamples = 0;                  // 最大样本数（0表示不限制）
};

// 音频信息
struct AudioInfo {
    int sampleRate = 0;
    int channels = 0;
    double duration = 0.0;
    int64_t totalSamples = 0;
    std::string codecName;
    std::string sampleFormat;
    int bitRate = 0;
};

class AudioDecoder {
public:
    AudioDecoder();
    ~AudioDecoder();
    
    // 初始化和配置
    bool initialize(const AudioDecoderConfig& config = AudioDecoderConfig());
    void destroy();
    bool isInitialized() const { return initialized_; }
    
    // 文件操作
    bool open(const std::string& filepath);
    void close();
    bool isOpened() const { return opened_; }
    
    // 获取信息
    AudioInfo getAudioInfo() const { return audioInfo_; }
    
    // 解码操作
    std::vector<AudioFrameData> getAllFrames();
    bool readNextFrame(AudioFrameData& frame);
    bool seekToTime(double seconds);
    bool seekToSample(int64_t sampleNumber);
    
    // 配置和控制
    void setThreadSafe(bool enable) { threadSafe_ = enable; }
    void setConfig(const AudioDecoderConfig& config) { config_ = config; }
    AudioDecoderConfig getConfig() const { return config_; }
    
    // 状态查询
    bool hasAudio() const { return audioStreamIndex_ != -1 && opened_; }
    double getCurrentTime() const { return currentTime_; }
    int64_t getCurrentSample() const { return currentSample_; }
    
private:
    // 内部方法
    bool initDecoder();
    bool initSwrContext();
    void cleanup();
    bool processPacket(AudioFrameData& frame);
    
    // FFmpeg组件
    AVFormatContext* formatCtx_ = nullptr;
    AVCodecContext* codecCtx_ = nullptr;
    AVPacket* packet_ = nullptr;
    AVFrame* frame_ = nullptr;
    SwrContext* swrCtx_ = nullptr;
    
    // 配置和状态
    AudioDecoderConfig config_;
    AudioInfo audioInfo_;
    bool initialized_ = false;
    bool opened_ = false;
    int audioStreamIndex_ = -1;
    double currentTime_ = 0.0;
    int64_t currentSample_ = 0;
    
    // 线程安全
    bool threadSafe_ = false;
    mutable std::mutex mutex_;
}; 