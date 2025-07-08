#ifndef MUXER_H
#define MUXER_H

#include <string>
#include <vector>
#include <mutex>
#include <algorithm>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

/**
 * @brief 封装器配置
 */
struct MuxerConfig {
    std::string outputPath;             // 输出文件路径
    std::string format = "mp4";         // 输出格式
    bool fastStart = true;              // MP4快速启动
    int64_t maxFileSize = 0;            // 最大文件大小（0=无限制）
    bool enableInterleaving = true;     // 启用时间交错
};

/**
 * @brief 流信息
 */
struct StreamInfo {
    int index = -1;                     // 流索引
    AVStream* stream = nullptr;         // AVStream指针
    AVCodecContext* codecContext = nullptr;  // 编码器上下文
    bool isVideo = false;               // 是否为视频流
    int64_t lastPts = AV_NOPTS_VALUE;   // 最后一个PTS
    int64_t lastDts = AV_NOPTS_VALUE;   // 最后一个DTS
};

/**
 * @brief 封装器
 * 
 * 负责将编码后的AVPacket封装为容器格式文件
 */
class Muxer {
public:
    Muxer();
    ~Muxer();
    
    /**
     * @brief 初始化封装器
     * @param config 封装配置
     * @return true 成功，false 失败
     */
    bool init(const MuxerConfig& config);
    
    /**
     * @brief 添加视频流
     * @param codecContext 视频编码器上下文
     * @return 流索引，-1表示失败
     */
    int addVideoStream(AVCodecContext* codecContext);
    
    /**
     * @brief 添加音频流
     * @param codecContext 音频编码器上下文
     * @return 流索引，-1表示失败
     */
    int addAudioStream(AVCodecContext* codecContext);
    
    /**
     * @brief 写入文件头
     * @return true 成功，false 失败
     */
    bool writeHeader();
    
    /**
     * @brief 写入数据包
     * @param packet 数据包
     * @param streamIndex 流索引
     * @return true 成功，false 失败
     */
    bool writePacket(AVPacket* packet, int streamIndex);
    
    /**
     * @brief 写入文件尾并关闭
     * @return true 成功，false 失败
     */
    bool finalize();
    
    /**
     * @brief 获取封装统计信息
     */
    struct Statistics {
        uint64_t totalPackets = 0;
        uint64_t videoPackets = 0;
        uint64_t audioPackets = 0;
        uint64_t totalBytes = 0;
        double totalDuration = 0.0;
        bool headerWritten = false;
        bool finalized = false;
    };
    
    Statistics getStatistics() const;
    
    /**
     * @brief 获取支持的输出格式列表
     * @return 格式名称列表
     */
    static std::vector<std::string> getSupportedFormats();
    
    /**
     * @brief 检查是否支持指定格式
     * @param format 格式名称
     * @return true 支持，false 不支持
     */
    static bool isFormatSupported(const std::string& format);

private:
    // FFmpeg组件
    AVFormatContext* formatContext_;
    const AVOutputFormat* outputFormat_;
    
    // 配置
    MuxerConfig config_;
    
    // 流管理
    std::vector<StreamInfo> streams_;
    int videoStreamIndex_;
    int audioStreamIndex_;
    
    // 状态
    bool initialized_;
    bool headerWritten_;
    bool finalized_;
    
    // 线程安全
    mutable std::mutex mutex_;
    
    // 统计信息
    mutable Statistics stats_;
    
    // 内部方法
    bool setupOutputFormat();
    bool setupOutputStream(AVCodecContext* codecContext, bool isVideo);
    bool validatePacket(AVPacket* packet, int streamIndex) const;
    void updateStatistics(AVPacket* packet, bool isVideo);
    void rescalePacketTimestamps(AVPacket* packet, int streamIndex);
    
    // 时间戳管理
    int64_t getNextPts(int streamIndex, int64_t currentPts);
    int64_t getNextDts(int streamIndex, int64_t currentDts);
    
    // 清理资源
    void cleanup();
};

#endif // MUXER_H 