#ifndef VIDEO_ENCODER_H
#define VIDEO_ENCODER_H

#include <string>
#include <memory>
#include "../DataStruct/FrameData.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

/**
 * @brief 视频编码器配置
 */
struct VideoEncoderConfig {
    std::string codec = "libx264";      // 编码器名称
    int width = 1920;                   // 视频宽度
    int height = 1080;                  // 视频高度
    int bitrate = 2000000;              // 码率 (bps)
    double frameRate = 30.0;            // 帧率
    std::string preset = "medium";      // 编码预设
    std::string profile = "high";       // 编码配置文件
    int gopSize = 30;                   // GOP大小
    AVPixelFormat pixelFormat = AV_PIX_FMT_YUV420P;  // 像素格式
    bool enableHardwareAccel = false;   // 硬件加速
    int threadCount = 0;                // 编码线程数
    int crf = -1;                       // CRF值 (-1=使用码率模式, 0-51=CRF模式, 0=无损)
};

/**
 * @brief 视频编码器
 * 
 * 负责将FrameData转换为编码后的AVPacket
 */
class VideoEncoder {
public:
    VideoEncoder();
    ~VideoEncoder();
    
    /**
     * @brief 初始化视频编码器
     * @param config 编码配置
     * @param timeBase 时间基准
     * @return true 成功，false 失败
     */
    bool init(const VideoEncoderConfig& config, AVRational timeBase);
    
    /**
     * @brief 编码视频帧
     * @param frameData 输入视频帧
     * @param packets 输出编码包列表
     * @return true 成功，false 失败
     */
    bool encode(const FrameData& frameData, std::vector<AVPacket*>& packets);
    
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
        double avgEncodingTime = 0.0;  // 平均编码时间（毫秒）
    };
    
    Statistics getStatistics() const;

private:
    // FFmpeg组件
    const AVCodec* codec_;
    AVCodecContext* codecContext_;
    AVFrame* frame_;
    SwsContext* swsContext_;
    int lastSrcWidth_;
    int lastSrcHeight_;
    
    // 配置
    VideoEncoderConfig config_;
    AVRational timeBase_;
    
    // 状态
    bool initialized_;
    int64_t frameIndex_;
    
    // 统计信息
    mutable Statistics stats_;
    
    // 内部方法
    bool setupCodec();
    bool setupSwsContext(int srcWidth, int srcHeight);
    AVFrame* convertFrameData(const FrameData& frameData);
    void updateStatistics(double encodingTime, size_t packetSize);
    void freePackets(std::vector<AVPacket*>& packets);
    
    // 像素格式转换
    AVPixelFormat getAVPixelFormat(const std::string& format) const;
    bool needsConversion(const FrameData& frameData) const;
};

#endif // VIDEO_ENCODER_H
