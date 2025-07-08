# SuperEigen Makefile
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -fPIC
OPENCV_FLAGS = $(shell pkg-config --cflags --libs opencv4)
INCLUDES = -I./include -I../DataStruct -I../Utils -I../Decoder/include -I../../onnx/onnxruntime-linux-x64-1.15.1/include
LIBS = -L../../onnx/onnxruntime-linux-x64-1.15.1/lib -lonnxruntime -lavformat -lavcodec -lavutil -lswscale -lswresample -lpthread $(OPENCV_FLAGS)

# 临时目录
BUILD_DIR = .build

# 源文件目录
SRC_DIR = src
DECODER_SRC_DIR = ../Decoder/src
UTILS_SRC_DIR = ../Utils

# 源文件
SRCS = $(SRC_DIR)/SuperResEngine.cpp \
       $(SRC_DIR)/SuperResConfig.cpp \
       $(SRC_DIR)/ModelSession.cpp \
       $(SRC_DIR)/PrePostProcessor.cpp \
       $(DECODER_SRC_DIR)/VideoDecoder.cpp \
       $(DECODER_SRC_DIR)/Decoder.cpp \
       $(DECODER_SRC_DIR)/AudioDecoder.cpp \
       $(UTILS_SRC_DIR)/Logger.cpp \
       $(UTILS_SRC_DIR)/LogUtils.cpp

# 目标文件（放在临时目录）
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

# 可执行文件
TARGET = test_performance

# 默认目标
all: $(BUILD_DIR) $(TARGET)

# 创建临时目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src $(BUILD_DIR)/../Decoder/src $(BUILD_DIR)/../Utils

# 编译可执行文件
$(TARGET): $(OBJS) test_performance.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPENCV_FLAGS) -o $(TARGET) test_performance.cpp $(OBJS) $(LIBS)

# 编译目标文件
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPENCV_FLAGS) -c $< -o $@

# 清理
clean:
	rm -rf $(BUILD_DIR) $(TARGET) *.png

# 运行测试
test: $(TARGET)
	export LD_LIBRARY_PATH="../../onnx/onnxruntime-linux-x64-1.15.1/lib:$$LD_LIBRARY_PATH" && ./$(TARGET) ../../resource/Vedio/vedio.mp4

.PHONY: all clean test 