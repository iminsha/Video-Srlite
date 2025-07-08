#ifndef AVSYNC_MANAGER_H
#define AVSYNC_MANAGER_H

#include <deque>
#include <variant>
#include <mutex>
#include "../DataStruct/FrameData.h"
#include "../DataStruct/AudioFrameData.h"

/**
 * @brief 音视频同步管理器
 * 
 * 负责基于时间戳的音视频帧同步调度，确保音画同步。
 * 核心原理：谁的时间戳更早，谁就先输出。
 */
class AVSyncManager {
public:
    // 统一的帧类型，可以是视频帧或音频帧
    using FrameVariant = std::variant<FrameData, AudioFrameData>;
    
    AVSyncManager();
    ~AVSyncManager();
    
    /**
     * @brief 推入视频帧到队列
     * @param frame 视频帧数据
     */
    void pushVideo(const FrameData& frame);
    
    /**
     * @brief 推入音频帧到队列
     * @param frame 音频帧数据
     */
    void pushAudio(const AudioFrameData& frame);
    
    /**
     * @brief 检查是否有帧可供输出
     * @return true 如果有帧可输出，false 否则
     */
    bool hasNext() const;
    
    /**
     * @brief 弹出时间戳最早的帧
     * @return 最早的帧（视频或音频）
     * @throws std::runtime_error 如果队列为空
     */
    FrameVariant popNext();
    
    /**
     * @brief 获取当前视频队列大小
     * @return 视频队列中的帧数
     */
    size_t getVideoQueueSize() const;
    
    /**
     * @brief 获取当前音频队列大小
     * @return 音频队列中的帧数
     */
    size_t getAudioQueueSize() const;
    
    /**
     * @brief 清空所有队列
     */
    void clear();
    
    /**
     * @brief 获取下一个要输出的帧的时间戳
     * @return 时间戳（秒），如果队列为空返回-1
     */
    double getNextTimestamp() const;
    
    /**
     * @brief 检查帧类型是否为视频帧
     * @param frame 帧变体
     * @return true 如果是视频帧
     */
    static bool isVideoFrame(const FrameVariant& frame);
    
    /**
     * @brief 检查帧类型是否为音频帧
     * @param frame 帧变体
     * @return true 如果是音频帧
     */
    static bool isAudioFrame(const FrameVariant& frame);
    
    /**
     * @brief 从帧变体中获取视频帧
     * @param frame 帧变体
     * @return 视频帧引用
     * @throws std::bad_variant_access 如果不是视频帧
     */
    static const FrameData& getVideoFrame(const FrameVariant& frame);
    
    /**
     * @brief 从帧变体中获取音频帧
     * @param frame 帧变体
     * @return 音频帧引用
     * @throws std::bad_variant_access 如果不是音频帧
     */
    static const AudioFrameData& getAudioFrame(const FrameVariant& frame);

private:
    // 视频帧队列
    std::deque<FrameData> videoQueue_;
    
    // 音频帧队列
    std::deque<AudioFrameData> audioQueue_;
    
    // 线程安全保护
    mutable std::mutex mutex_;
    
    /**
     * @brief 获取视频队列前端帧的时间戳
     * @return 时间戳，如果队列为空返回无穷大
     */
    double getVideoFrontTimestamp() const;
    
    /**
     * @brief 获取音频队列前端帧的时间戳
     * @return 时间戳，如果队列为空返回无穷大
     */
    double getAudioFrontTimestamp() const;
};

#endif // AVSYNC_MANAGER_H 