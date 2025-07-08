# VideoSR-Lite 解码器 Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
INCLUDES = -I. -I../DataStruct -I../Utils -I/usr/include/opencv4
LIBS = -lavformat -lavcodec -lavutil -lswscale -lswresample -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lpthread

SOURCES = src/VideoDecoder.cpp src/AudioDecoder.cpp src/Decoder.cpp ../Utils/Logger.cpp ../Utils/LogUtils.cpp

# 默认目标：完整测试
all: decoder_test

# 完整测试程序
decoder_test: $(SOURCES) test_decoder.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o decoder_test $(SOURCES) test_decoder.cpp $(LIBS)

# 测试运行
test: decoder_test
	@echo "运行解码器测试..."
	./decoder_test ../../resource/Vedio/vedio.mp4

clean:
	rm -f decoder_test

.PHONY: all clean test 