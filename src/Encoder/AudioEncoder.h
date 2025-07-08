#ifndef AUDIO_ENCODER_H
#define AUDIO_ENCODER_H

#include <string>
#include <memory>
#include "../DataStruct/AudioFrameData.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libswresample/swresample.h>
}

/**
 * @brief 音频编码器配置
 */
struct AudioEncoderConfig {
    std::string codec = "aac";          // 编码器名称
    int sampleRate = 48000;             // 采样率
    int channels = 2;                   // 声道数
    int bitrate = 128000;               // 码率 (bps)
    AVSampleFormat sampleFormat = AV_SAMPLE_FMT_FLTP;  // 样本格式
    uint64_t channelLayout = AV_CH_LAYOUT_STEREO;  // 声道布局（旧版FFmpeg）
    int frameSize = 1024;               // 每帧样本数
    bool enableStreamCopy = false;      // 是否启用流拷贝（跳过编码）
};

/**
 * @brief 音频编码器
 * 
 * 负责将AudioFrameData转换为编码后的AVPacket
 */
class AudioEncoder {
public:
    AudioEncoder();
    ~AudioEncoder();
    
    /**
     * @brief 初始化音频编码器
     * @param config 编码配置
     * @param timeBase 时间基准
     * @return true 成功，false 失败
     */
    bool init(const AudioEncoderConfig& config, AVRational timeBase);
    
    /**
     * @brief 编码音频帧
     * @param frameData 输入音频帧
     * @param packets 输出编码包列表
     * @return true 成功，false 失败
     */
    bool encode(const AudioFrameData& frameData, std::vector<AVPacket*>& packets);
    
    /**
     * @brief 刷新编码器，输出剩余帧
     * @param packets 输出编码包列表
     * @return true 成功，false 失败
     */
    bool flush(std::vector<AVPacket*>& packets);
    
    /**
     * @brief 关闭编码器
     */
    void close();
    
    /**
     * @brief 获取编码器上下文（用于Muxer）
     * @return AVCodecContext指针
     */
    AVCodecContext* getCodecContext() const { return codecContext_; }
    
    /**
     * @brief 获取编码统计信息
     */
    struct Statistics {
        uint64_t framesEncoded = 0;
        uint64_t bytesEncoded = 0;
        uint64_t samplesEncoded = 0;
        double avgEncodingTime = 0.0;  // 平均编码时间（毫秒）
    };
    
    Statistics getStatistics() const;

private:
    // FFmpeg组件
    const AVCodec* codec_;
    AVCodecContext* codecContext_;
    AVFrame* frame_;
    SwrContext* swrContext_;
    
    // 配置
    AudioEncoderConfig config_;
    AVRational timeBase_;
    
    // 状态
    bool initialized_;
    int64_t sampleIndex_;
    
    // 重采样缓冲区
    uint8_t** resampleBuffer_;
    int resampleBufferSize_;
    
    // 统计信息
    mutable Statistics stats_;
    
    // 内部方法
    bool setupCodec();
    bool setupSwrContext();
    AVFrame* convertAudioFrameData(const AudioFrameData& frameData);
    bool resampleAudio(const AudioFrameData& frameData, AVFrame* outputFrame);
    void updateStatistics(double encodingTime, size_t packetSize, int samples);
    void freePackets(std::vector<AVPacket*>& packets);
    void freeResampleBuffer();
    
    // 格式转换
    AVSampleFormat getAVSampleFormat(const std::string& format) const;
    uint64_t getAVChannelLayout(int channels) const;
    bool needsResampling(const AudioFrameData& frameData) const;
    
    // 流拷贝模式
    bool processStreamCopy(const AudioFrameData& frameData, std::vector<AVPacket*>& packets);
};

#endif // AUDIO_ENCODER_H
