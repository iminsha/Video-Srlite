# AVSyncManager Makefile
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
OPENCV_FLAGS = $(shell pkg-config --cflags --libs opencv4)
INCLUDES = -I../Utils -I../DataStruct

# 源文件
SOURCES = AVSyncManager.cpp ../Utils/Logger.cpp ../Utils/LogUtils.cpp
TEST_SOURCES = test_avsync.cpp

# 目标
TARGET = test_avsync

# 默认目标
all: $(TARGET)

# 编译测试程序
$(TARGET): $(TEST_SOURCES) $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(TEST_SOURCES) $(SOURCES) $(OPENCV_FLAGS)

# 清理
clean:
	rm -f $(TARGET)

# 运行测试
test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test 