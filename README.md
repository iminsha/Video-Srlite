# VideoSR-Lite

基于 RealESRGAN 的视频超分辨率处理工具，支持视频和图片的 2x 超分辨率处理。

## 🚀 快速开始

### 1. 一键编译
```bash
./build.sh
```

### 2. 直接使用

#### 单张图片超分
```bash
./build/bin/run_sr_image 输入图片 输出图片
```

示例：
```bash
./build/bin/run_sr_image photo.jpg enhanced_photo.png
./build/bin/run_sr_image input.jpeg output.png
```

#### 视频处理流水线
```bash
./build/bin/test_pipeline 输入视频 输出视频
```

示例：
```bash
./build/bin/test_pipeline input.mp4 output.mp4
./build/bin/test_pipeline video.avi enhanced_video.mp4
```

## 🛠️ 系统要求

### 依赖安装（Ubuntu/Debian）
```bash
sudo apt update
sudo apt install -y libopencv-dev libavformat-dev libavcodec-dev \
                    libavutil-dev libswscale-dev libswresample-dev \
                    pkg-config build-essential
```

## 📁 项目结构
```
VideoSR-Lite/
├── build/bin/           # 编译后的可执行文件
├── onnx/               # AI模型文件
├── resource/           # 测试资源
├── src/                # 源代码
├── build.sh            # 自动化构建脚本
└── README.md           # 项目说明
```

## ✨ 特性

- **2x 超分辨率**: 基于 RealESRGAN 模型
- **一键编译**: 自动解决所有依赖和路径问题
- **直接运行**: 无需设置环境变量
- **自动路径**: 自动查找模型文件
- **高质量**: H.264 编码，CRF=18

---

编译完成后，可执行文件已配置好所有路径，直接使用即可！ 