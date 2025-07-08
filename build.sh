#!/bin/bash

# VideoSR-Lite 自动化编译脚本
# 一键编译，解决动态库路径问题

set -e  # 遇到错误立即退出

echo "=========================================="
echo "VideoSR-Lite 自动化编译"
echo "=========================================="

# 项目根目录
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_ROOT"

# 创建构建目录
BUILD_DIR="$PROJECT_ROOT/build"
BIN_DIR="$BUILD_DIR/bin"

echo "创建构建目录..."
mkdir -p "$BUILD_DIR"
mkdir -p "$BIN_DIR"

# 编译配置
CXX=g++
CXXFLAGS="-std=c++17 -O2 -Wall -Wextra -fPIC"
INCLUDES="-Isrc -Isrc/Decoder/include -Isrc/SuperEigen/include -Isrc/SyncVA -Isrc/Encoder -Isrc/Utils -Isrc/DataStruct -Isrc/Processing"
OPENCV_FLAGS="$(pkg-config --cflags --libs opencv4)"
FFMPEG_FLAGS="-lavformat -lavcodec -lavutil -lswscale -lswresample"
ONNX_PATH="onnx/onnxruntime-linux-x64-1.15.1"
ONNX_FLAGS="-I${ONNX_PATH}/include -L${ONNX_PATH}/lib -lonnxruntime"

# 设置 RPATH，让可执行文件自动找到动态库
# $ORIGIN 是可执行文件所在目录，从 build/bin 到项目根目录是 ../..
RPATH_FLAGS="-Wl,-rpath,\$ORIGIN/../../${ONNX_PATH}/lib"

LIBS="$OPENCV_FLAGS $FFMPEG_FLAGS $ONNX_FLAGS $RPATH_FLAGS -pthread"

# 通用源文件
DECODER_SOURCES="src/Decoder/src/VideoDecoder.cpp src/Decoder/src/AudioDecoder.cpp src/Decoder/src/Decoder.cpp"
SUPERRES_SOURCES="src/SuperEigen/src/SuperResEngine.cpp src/SuperEigen/src/ModelSession.cpp src/SuperEigen/src/PrePostProcessor.cpp src/SuperEigen/src/SuperResConfig.cpp"
SYNC_SOURCES="src/SyncVA/AVSyncManager.cpp"
ENCODER_SOURCES="src/Encoder/Encoder.cpp src/Encoder/VideoEncoder.cpp src/Encoder/AudioEncoder.cpp src/Encoder/Muxer.cpp"
UTILS_SOURCES="src/Utils/Logger.cpp src/Utils/LogUtils.cpp src/Utils/FileUtils.cpp"
PROCESSING_SOURCES="src/Processing/SuperResolution.cpp"

# 编译 test_pipeline (完整视频处理流水线)
echo "=========================================="
echo "编译 test_pipeline (视频处理流水线)"
echo "=========================================="
$CXX $CXXFLAGS $INCLUDES -o "$BIN_DIR/test_pipeline" \
    src/test_pipeline.cpp \
    $DECODER_SOURCES $SUPERRES_SOURCES $SYNC_SOURCES $ENCODER_SOURCES $UTILS_SOURCES \
    $LIBS
echo "✅ test_pipeline 编译完成"

# 编译 run_sr_image (单张图片超分)
echo "=========================================="
echo "编译 run_sr_image (单张图片超分)"
echo "=========================================="
$CXX $CXXFLAGS $INCLUDES -o "$BIN_DIR/run_sr_image" \
    src/tools/run_sr_image.cpp \
    $SUPERRES_SOURCES $PROCESSING_SOURCES $UTILS_SOURCES \
    $LIBS
echo "✅ run_sr_image 编译完成"

# 编译 GUI 应用程序
echo "=========================================="
echo "编译 VideoSR-Lite GUI"
echo "=========================================="

# 检查Qt5
if pkg-config --exists Qt5Core Qt5Widgets Qt5Gui; then
    echo "📋 检测到Qt5，开始编译GUI..."
    
    # 使用CMake编译GUI（更适合Qt项目）
    cd "$BUILD_DIR"
    cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_GUI=ON ..
    make -j$(nproc) VideoSRLiteGUI
    
    if [ -f "bin/VideoSRLiteGUI" ]; then
        echo "✅ VideoSR-Lite GUI 编译完成"
    else
        echo "❌ GUI 编译失败"
    fi
    
    cd "$PROJECT_ROOT"
else
    echo "⚠️  未检测到Qt5开发包，跳过GUI编译"
    echo "   安装命令: sudo apt-get install qtbase5-dev qttools5-dev-tools"
fi

echo "=========================================="
echo "编译完成！"
echo "=========================================="

# 显示生成的文件
echo "生成的可执行文件:"
ls -la "$BIN_DIR"

echo ""
echo "使用方法:"
echo "  🖼️  单张图片超分: ./build/bin/run_sr_image input.jpg output.png"
echo "  🎬 视频处理流水线: ./build/bin/test_pipeline [input.mp4] [output.mp4]"
if [ -f "$BIN_DIR/VideoSRLiteGUI" ]; then
echo "  🖥️  图形界面应用: ./build/bin/VideoSRLiteGUI"
fi
echo ""
echo "🚀 快速启动GUI:"
echo "  cd build && ./bin/VideoSRLiteGUI"
echo ""
echo "可执行文件已配置动态库路径，可直接运行！" 