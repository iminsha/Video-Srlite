#ifndef ENCODER_H
#define ENCODER_H

#include <variant>
#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include <chrono>
#include "../DataStruct/FrameData.h"
#include "../DataStruct/AudioFrameData.h"
#include "VideoEncoder.h"
#include "AudioEncoder.h"
#include "Muxer.h"

/**
 * @brief 编码器配置结构
 */
struct EncoderConfig {
    // 输出文件配置
    std::string outputPath;           // 输出文件路径
    std::string format = "mp4";       // 输出格式 (mp4, mkv, ts)
    
    // 视频编码配置
    std::string videoCodec = "libx264";   // 视频编码器
    int videoBitrate = 2000000;           // 视频码率 (bps)
    int videoWidth = 0;                   // 视频宽度（0=自动从输入帧获取）
    int videoHeight = 0;                  // 视频高度（0=自动从输入帧获取）
    double videoFrameRate = 30.0;         // 视频帧率
    std::string videoPreset = "medium";   // 编码预设
    int videoCRF = -1;                    // CRF值 (-1=使用码率模式, 0-51=CRF模式, 0=无损)
    
    // 音频编码配置
    std::string audioCodec = "aac";       // 音频编码器
    int audioBitrate = 128000;            // 音频码率 (bps)
    int audioSampleRate = 48000;          // 音频采样率
    int audioChannels = 2;                // 音频声道数
    
    // 高级配置
    bool enableHardwareAccel = false;     // 硬件加速
    int threadCount = 0;                  // 编码线程数 (0=auto)
    bool fastStart = true;                // MP4快速启动
};

/**
 * @brief 统一编码器状态
 */
enum class EncoderState {
    Uninitialized,  // 未初始化
    Running,        // 运行中
    Flushed,        // 已刷新
    Closed          // 已关闭
};

/**
 * @brief 统一编码控制器
 * 
 * 管理整体编码生命周期，接收同步后的帧进行编码
 */
class Encoder {
public:
    // 帧类型定义
    using FrameVariant = std::variant<FrameData, AudioFrameData>;
    
    Encoder();
    ~Encoder();
    
    /**
     * @brief 初始化编码器
     * @param config 编码配置
     * @return true 成功，false 失败
     */
    bool init(const EncoderConfig& config);
    
    /**
     * @brief 推入音视频帧进行编码
     * @param frame 帧数据（视频或音频）
     * @return true 成功，false 失败
     */
    bool push(const FrameVariant& frame);
    
    /**
     * @brief 刷新编码器缓存
     * @return true 成功，false 失败
     */
    bool flush();
    
    /**
     * @brief 关闭编码器并释放资源
     * @return true 成功，false 失败
     */
    bool close();
    
    /**
     * @brief 获取当前状态
     * @return 编码器状态
     */
    EncoderState getState() const;
    
    /**
     * @brief 获取编码统计信息
     */
    struct Statistics {
        uint64_t videoFramesEncoded = 0;
        uint64_t audioFramesEncoded = 0;
        uint64_t totalBytesWritten = 0;
        double totalDuration = 0.0;
        double encodingSpeed = 0.0;  // 编码速度倍数
    };
    
    Statistics getStatistics() const;
    
    /**
     * @brief 检查是否支持指定的编码器
     * @param codecName 编码器名称
     * @param isVideo true=视频编码器，false=音频编码器
     * @return true 支持，false 不支持
     */
    static bool isCodecSupported(const std::string& codecName, bool isVideo);
    
    /**
     * @brief 获取支持的编码器列表
     * @param isVideo true=视频编码器，false=音频编码器
     * @return 编码器名称列表
     */
    static std::vector<std::string> getSupportedCodecs(bool isVideo);

private:
    // 状态管理
    EncoderState state_;
    mutable std::mutex mutex_;
    
    // 配置
    EncoderConfig config_;
    
    // 编码器组件
    std::unique_ptr<VideoEncoder> videoEncoder_;
    std::unique_ptr<AudioEncoder> audioEncoder_;
    std::unique_ptr<Muxer> muxer_;
    
    // 统计信息
    mutable Statistics stats_;
    std::chrono::steady_clock::time_point startTime_;
    
    // 内部方法
    bool initializeComponents();
    bool processVideoFrame(const FrameData& frame);
    bool processAudioFrame(const AudioFrameData& frame);
    void updateStatistics();
    void setState(EncoderState newState);
    bool validateState(EncoderState requiredState) const;
};

#endif // ENCODER_H
