# VideoSR-Lite 简化解码器模块

## 概述

这是一个极简的音视频解码器模块，专注于核心功能：将媒体文件解码为视频帧集合和音频帧集合。

## 文件结构

```
Decoder/
├── VideoDecoder.h      # 视频解码器头文件
├── VideoDecoder.cpp    # 视频解码器实现
├── AudioDecoder.h      # 音频解码器头文件  
├── AudioDecoder.cpp    # 音频解码器实现
├── Decoder.h           # 统一解码器头文件
├── Decoder.cpp         # 统一解码器实现
├── test_decoder.cpp    # 测试程序
└── Makefile           # 构建文件
```

## 主要功能

1. **VideoDecoder** - 视频解码器
   - 打开视频文件
   - 解码所有视频帧为 `std::vector<FrameData>`
   - 每帧包含时间戳信息

2. **AudioDecoder** - 音频解码器
   - 打开音频文件
   - 解码所有音频帧为 `std::vector<AudioFrameData>`
   - 每帧包含时间戳信息

3. **Decoder** - 统一解码器
   - 同时管理视频和音频解码器
   - 提供统一的接口

## 使用方法

### 基本使用

```cpp
#include "Decoder.h"

// 创建解码器
Decoder decoder;

// 打开媒体文件
if (decoder.open("video.mp4")) {
    // 获取视频帧集合
    if (decoder.hasVideo()) {
        std::vector<FrameData> videoFrames = decoder.getVideoFrames();
        std::cout << "视频帧数: " << videoFrames.size() << std::endl;
    }
    
    // 获取音频帧集合
    if (decoder.hasAudio()) {
        std::vector<AudioFrameData> audioFrames = decoder.getAudioFrames();
        std::cout << "音频帧数: " << audioFrames.size() << std::endl;
    }
    
    // 关闭解码器
    decoder.close();
}
```

### 多线程支持

```cpp
// 启用线程安全模式
decoder.setThreadSafe(true);
```

## 编译和测试

```bash
# 编译
make

# 测试统一解码器（默认）
make test

# 只测试视频解码
make test-video

# 只测试音频解码
make test-audio

# 测试所有功能
make test-all

# 清理
make clean
```

## 数据结构

### FrameData (视频帧)
- `cv::Mat image` - 图像数据
- `double timestamp` - 时间戳（秒）
- `int frameIndex` - 帧索引
- `int width, height` - 分辨率

### AudioFrameData (音频帧)
- `std::vector<uint8_t> data` - 音频数据
- `double timestamp` - 时间戳（秒）
- `int sampleRate` - 采样率
- `int channels` - 声道数
- `int nbSamples` - 采样数

## 注意事项

1. 解码器会一次性将所有帧加载到内存，适用于中小型文件
2. 对于大文件，建议分批处理或使用流式处理
3. 音视频同步需要在其他模块中处理
4. 默认输出格式：
   - 视频：BGR24 (OpenCV格式)
   - 音频：原始PCM数据

## 测试文件要求

测试文件应包含音频和视频流。如果测试文件只有视频没有音频，可以使用FFmpeg添加音频：

```bash
# 为视频添加静音音频轨道
ffmpeg -i input_video.mp4 -f lavfi -i anullsrc=r=44100:cl=stereo -shortest -c:v copy output_with_audio.mp4

# 或者添加一个音频文件
ffmpeg -i video.mp4 -i audio.mp3 -c:v copy -c:a aac -shortest output.mp4
``` 