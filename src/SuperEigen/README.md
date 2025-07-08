# SuperEigen 超分辨率模块

## 概述
SuperEigen是一个基于ONNX Runtime的视频超分辨率处理模块，支持对视频帧进行2倍超分辨率增强。

## 目录结构
```
SuperEigen/
├── src/                    # 源代码（纯净，无.o文件）
│   ├── SuperResEngine.cpp  # 主引擎实现
│   ├── SuperResConfig.cpp  # 配置管理
│   ├── ModelSession.cpp    # ONNX模型会话
│   └── PrePostProcessor.cpp # 预处理和后处理
├── include/                # 头文件
├── .build/                 # 编译临时文件（自动生成）
│   └── *.o                 # 目标文件
├── test_performance.cpp    # 性能测试程序
├── Makefile               # 构建文件
└── README.md              # 本文档
```

## 编译和运行

### 编译
```bash
# 在SuperEigen目录下
make clean
make -j$(nproc)
```

### 运行测试程序
```bash
# 设置ONNX Runtime库路径并运行
export LD_LIBRARY_PATH="../../onnx/onnxruntime-linux-x64-1.15.1/lib:$LD_LIBRARY_PATH"
./test_performance <视频文件路径>

# 示例
./test_performance ../../resource/Vedio/vedio.mp4
```

## 功能特性

### 核心功能
- 支持2倍超分辨率处理
- 基于RealESRGAN模型
- CPU和GPU支持（当前主要测试CPU）
- 智能内存管理，自动处理大尺寸图像

### 性能特点
- 输入分辨率：720x1280
- 输出分辨率：1440x2560
- 处理速度：约3.5秒/帧（CPU）
- 内存安全：自动缩放超大图像

## 输出文件

测试程序会在当前目录生成以下文件：
- `original_frame_N.png` - 原始帧图像
- `super_res_frame_N.png` - 超分辨率处理后的图像

其中N为帧序号（1-5）。

## 技术细节

### 依赖库
- OpenCV 4.x
- ONNX Runtime 1.15.1
- FFmpeg (libavformat, libavcodec, libavutil, libswscale, libswresample)

### 模型要求
- 模型文件：`RealESRGAN_x2plus.fp16.onnx`
- 位置：`../../onnx/RealESRGAN_x2plus.fp16.onnx`
- 输入要求：图像尺寸必须能被4整除

### 内存优化
程序会自动检测输入图像尺寸：
- 大于512像素的图像会被智能缩放处理
- 确保处理过程中的内存安全
- 最终输出会缩放到目标尺寸

## 使用示例

### 基本使用
```cpp
#include "SuperResEngine.h"

// 初始化引擎
SuperResEngine engine;
engine.initializeDefault();

// 处理单帧
FrameData inputFrame = /* 从解码器获取 */;
FrameData outputFrame = engine.processFrame(inputFrame);

// 保存结果
cv::imwrite("output.png", outputFrame.image);
```

### 批量处理
```cpp
std::vector<FrameData> inputFrames = /* 准备输入帧 */;
std::vector<FrameData> outputFrames = engine.processBatch(inputFrames);
```

## 性能测试结果

### 测试环境
- 系统：Linux
- 处理器：CPU（4线程）
- 模型：RealESRGAN_x2plus.fp16.onnx

### 测试结果
- 平均处理时间：3.50秒/帧
- 处理速度：0.285 fps
- 实时倍率：0.0095x（无法实时处理）
- 超分倍率：2x
- 内存使用：安全且稳定

### 输出质量
- 原始图像：720x1280 (约1.6MB PNG)
- 超分图像：1440x2560 (约5MB PNG)
- 质量提升：显著的细节增强

## 故障排除

### 常见问题
1. **找不到模型文件**
   - 确保`RealESRGAN_x2plus.fp16.onnx`在正确位置
   - 检查相对路径是否正确

2. **库文件找不到**
   - 设置正确的`LD_LIBRARY_PATH`
   - 确保ONNX Runtime库文件存在

3. **内存不足**
   - 程序会自动处理大图像
   - 如仍有问题，可调整`maxSafeSize`参数

### 调试信息
程序会输出详细的处理信息，包括：
- 输入/输出尺寸
- 处理时间
- 内存使用情况
- 错误信息

## 扩展开发

### 添加新模型
1. 将ONNX模型文件放入`onnx`目录
2. 修改`getAvailableModels()`方法
3. 更新配置文件

### GPU支持
程序已预留GPU支持接口，需要：
1. 安装CUDA版本的ONNX Runtime
2. 修改编译配置
3. 测试GPU初始化

## 许可证
请参考项目根目录的许可证文件。 