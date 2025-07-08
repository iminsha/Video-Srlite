# 视频处理流水线 Makefile
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -fPIC

# 包含路径
INCLUDES = -I. \
           -IDecoder/include \
           -ISuperEigen/include \
           -ISyncVA \
           -IEncoder \
           -IUtils \
           -IDataStruct

# OpenCV和FFmpeg库
OPENCV_FLAGS = $(shell pkg-config --cflags --libs opencv4)
FFMPEG_FLAGS = -lavformat -lavcodec -lavutil -lswscale -lswresample

# ONNX Runtime路径
ONNX_PATH = ../onnx/onnxruntime-linux-x64-1.15.1
ONNX_FLAGS = -I$(ONNX_PATH)/include -L$(ONNX_PATH)/lib -lonnxruntime

# 所有库标志
LIBS = $(OPENCV_FLAGS) $(FFMPEG_FLAGS) $(ONNX_FLAGS) -pthread

# 源文件
DECODER_SOURCES = Decoder/src/VideoDecoder.cpp \
                  Decoder/src/AudioDecoder.cpp \
                  Decoder/src/Decoder.cpp

SUPERRES_SOURCES = SuperEigen/src/SuperResEngine.cpp \
                   SuperEigen/src/ModelSession.cpp \
                   SuperEigen/src/PrePostProcessor.cpp \
                   SuperEigen/src/SuperResConfig.cpp

SYNC_SOURCES = SyncVA/AVSyncManager.cpp

ENCODER_SOURCES = Encoder/Encoder.cpp \
                  Encoder/VideoEncoder.cpp \
                  Encoder/AudioEncoder.cpp \
                  Encoder/Muxer.cpp

UTILS_SOURCES = Utils/Logger.cpp \
                Utils/LogUtils.cpp \
                Utils/FileUtils.cpp

# 主程序源文件
MAIN_SOURCE = test_pipeline.cpp

# 所有源文件
ALL_SOURCES = $(MAIN_SOURCE) $(DECODER_SOURCES) $(SUPERRES_SOURCES) \
              $(SYNC_SOURCES) $(ENCODER_SOURCES) $(UTILS_SOURCES)

# 目标程序
TARGET = test_pipeline

# 默认目标
all: $(TARGET)

# 编译主程序
$(TARGET): $(ALL_SOURCES)
	@echo "Compiling video processing pipeline..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(ALL_SOURCES) $(LIBS)
	@echo "Build completed: $(TARGET)"

# 清理
clean:
	rm -f $(TARGET)
	@echo "Cleaned build files"

# 运行测试
test: $(TARGET)
	@echo "Running pipeline test..."
	./$(TARGET)

# 运行测试（指定输入文件）
test-input: $(TARGET)
	@echo "Running pipeline test with custom input..."
	./$(TARGET) ../resource/Vedio/vedio.mp4 ../resource/output_enhanced.mp4

# 检查依赖
check-deps:
	@echo "Checking dependencies..."
	@pkg-config --exists opencv4 && echo "✓ OpenCV found" || echo "✗ OpenCV not found"
	@ldconfig -p | grep -q libavformat && echo "✓ FFmpeg found" || echo "✗ FFmpeg not found"
	@test -f $(ONNX_PATH)/lib/libonnxruntime.so && echo "✓ ONNX Runtime found" || echo "✗ ONNX Runtime not found"
	@test -f ../resource/Vedio/vedio.mp4 && echo "✓ Test video found" || echo "✗ Test video not found"

# 安装依赖（Ubuntu/Debian）
install-deps:
	@echo "Installing dependencies..."
	sudo apt update
	sudo apt install -y libopencv-dev libavformat-dev libavcodec-dev libavutil-dev libswscale-dev libswresample-dev

# 显示帮助
help:
	@echo "Available targets:"
	@echo "  all          - Build the pipeline"
	@echo "  clean        - Clean build files"
	@echo "  test         - Run pipeline test with default input"
	@echo "  test-input   - Run pipeline test with custom input"
	@echo "  check-deps   - Check if all dependencies are available"
	@echo "  install-deps - Install system dependencies"
	@echo "  help         - Show this help"

.PHONY: all clean test test-input check-deps install-deps help 