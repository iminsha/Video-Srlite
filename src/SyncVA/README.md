# AVSyncManager - 音视频同步管理器

## 概述

`AVSyncManager` 是一个基于时间戳的音视频帧同步调度器，用于确保音画同步。它采用时间戳驱动的调度机制，谁的时间戳更早，谁就先输出。

## 核心特性

- **时间戳驱动**：基于帧的时间戳（秒）进行同步调度
- **统一接口**：使用 `std::variant<FrameData, AudioFrameData>` 统一处理音视频帧
- **线程安全**：内部使用互斥锁保护，支持多线程环境
- **实时处理**：支持动态推入和弹出，适合实时流处理

## 工作流程

```
           ┌────────────┐
           │  Decoder   │
           └────┬───────┘
                ▼
        ┌──────────────┐
        │ 超分 / 降噪模块 │
        └────┬───────┬──┘
             ▼       ▼
      [视频帧队列]  [音频帧队列]
             ▼       ▼
        ┌───────────────┐
        │ AVSyncManager │ ← 负责同步调度
        └────┬──────────┘
             ▼
        ┌─────────────┐
        │   Encoder   │
        └─────────────┘
```

## API 接口

### 基本操作

```cpp
#include "AVSyncManager.h"

// 创建同步管理器
AVSyncManager syncManager;

// 推入帧
syncManager.pushVideo(videoFrame);    // 推入视频帧
syncManager.pushAudio(audioFrame);    // 推入音频帧

// 检查和弹出
if (syncManager.hasNext()) {
    auto frame = syncManager.popNext();  // 弹出时间戳最早的帧
    
    // 判断帧类型
    if (AVSyncManager::isVideoFrame(frame)) {
        const auto& videoFrame = AVSyncManager::getVideoFrame(frame);
        // 处理视频帧
    } else if (AVSyncManager::isAudioFrame(frame)) {
        const auto& audioFrame = AVSyncManager::getAudioFrame(frame);
        // 处理音频帧
    }
}
```

### 状态查询

```cpp
// 队列状态
size_t videoCount = syncManager.getVideoQueueSize();
size_t audioCount = syncManager.getAudioQueueSize();

// 下一帧时间戳
double nextTimestamp = syncManager.getNextTimestamp();

// 清空队列
syncManager.clear();
```

## 同步原理

### 时间戳比较
- 同时比较视频队列和音频队列的首帧时间戳
- 选择时间戳更小（更早）的帧先输出
- 如果某个队列为空，则输出另一个队列的帧

### 示例场景
假设有以下帧序列：
- 视频帧：0.000s, 0.033s, 0.067s, 0.100s
- 音频帧：0.000s, 0.021s, 0.043s, 0.064s, 0.085s

输出顺序将是：
1. 视频帧 @ 0.000s (与音频帧时间戳相同，视频优先)
2. 音频帧 @ 0.000s  
3. 音频帧 @ 0.021s
4. 视频帧 @ 0.033s
5. 音频帧 @ 0.043s
6. 音频帧 @ 0.064s
7. 视频帧 @ 0.067s
8. 音频帧 @ 0.085s
9. 视频帧 @ 0.100s

## 编译和测试

### 编译
```bash
cd VideoSR-Lite/src/Core
make
```

### 运行测试
```bash
make test
# 或者
./test_avsync
```

## 测试结果示例

```
=== AVSyncManager 测试程序 ===
序号 | 类型  | 时间戳  | 帧信息
-----|-------|---------|------------------
  1 | VIDEO | 0.000s | Frame 0 | 100x100
  2 | AUDIO | 0.000s | 1024 samples | 48000Hz, 2ch
  3 | AUDIO | 0.021s | 1024 samples | 48000Hz, 2ch
  4 | VIDEO | 0.033s | Frame 1 | 100x100
  5 | AUDIO | 0.043s | 1024 samples | 48000Hz, 2ch
  ...
```

## 使用场景

1. **视频处理流水线**：在解码后、编码前进行音视频同步
2. **实时流处理**：动态接收和输出音视频帧
3. **后处理同步**：确保超分辨率或降噪处理后的帧顺序正确

## 注意事项

1. **时间戳精度**：确保输入帧的时间戳精确，建议使用秒为单位的浮点数
2. **内存管理**：大量帧缓存可能占用较多内存，及时处理避免积压
3. **线程安全**：虽然内部线程安全，但建议在单线程环境下使用以获得最佳性能
4. **异常处理**：空队列调用 `popNext()` 会抛出异常，使用前检查 `hasNext()` 