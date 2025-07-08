#!/bin/bash

# VideoSR-Lite UI 构建脚本
echo "🚀 开始构建 VideoSR-Lite UI 模块..."

# 检查依赖
echo "📋 检查构建依赖..."

# 检查Qt5
if ! pkg-config --exists Qt5Core; then
    echo "❌ 未找到 Qt5 开发包，请安装："
    echo "   sudo apt-get install qtbase5-dev qttools5-dev-tools"
    exit 1
fi

# 检查OpenCV
if ! pkg-config --exists opencv4; then
    echo "❌ 未找到 OpenCV4，请安装："
    echo "   sudo apt-get install libopencv-dev"
    exit 1
fi

# 检查FFmpeg
if ! pkg-config --exists libavcodec; then
    echo "❌ 未找到 FFmpeg 开发包，请安装："
    echo "   sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev"
    exit 1
fi

echo "✅ 依赖检查通过"

# 创建构建目录
echo "📁 准备构建目录..."
mkdir -p build
cd build

# 运行CMake配置
echo "⚙️  配置CMake..."
cmake -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_GUI=ON \
      -DCMAKE_INSTALL_PREFIX=./install \
      ..

if [ $? -ne 0 ]; then
    echo "❌ CMake 配置失败"
    exit 1
fi

# 编译
echo "🔨 开始编译..."
make -j$(nproc) VideoSRLiteGUI

if [ $? -ne 0 ]; then
    echo "❌ 编译失败"
    exit 1
fi

echo "✅ 编译成功！"

# 检查可执行文件
if [ -f "./bin/VideoSRLiteGUI" ]; then
    echo "🎯 可执行文件生成: ./build/bin/VideoSRLiteGUI"
    echo ""
    echo "🚀 启动命令："
    echo "   cd build && ./bin/VideoSRLiteGUI"
    echo ""
    echo "📝 使用说明："
    echo "   1. 点击'添加文件'选择图片或视频"
    echo "   2. 选择文件后点击'加载预览'查看效果"
    echo "   3. 调整参数后点击'开始处理'"
    echo "   4. 处理结果将保存在原文件同目录"
else
    echo "❌ 可执行文件未生成"
    exit 1
fi

echo ""
echo "🎉 VideoSR-Lite UI 构建完成！" 