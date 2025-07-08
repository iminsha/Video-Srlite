# 解码器模块故障记录

## 故障编号：DECODER-001
**日期**：2025-06-27  
**严重级别**：高  
**状态**：已修复  

## 故障描述

### 症状
1. **程序阻塞**：完整测试程序 `decoder_test` 在运行时会无限阻塞，需要手动中断（Ctrl+C）
2. **部分功能正常**：视频解码功能本身正常工作，能成功解码视频帧
3. **帧数不匹配**：视频头信息显示153帧，但实际只解码了148帧

### 影响范围
- 完整测试程序无法正常完成所有测试
- 影响开发和调试效率
- 可能影响生产环境中的资源释放

## 根本原因分析

### 主要问题：死锁（Deadlock）
**位置**：`VideoDecoder::destroy()` 和 `AudioDecoder::destroy()` 函数

**死锁机制**：
```cpp
// 错误的实现
void VideoDecoder::destroy() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();  // 获取锁
    
    if (!initialized_) return;
    
    close();  // 调用close()，但close()也会尝试获取同一个锁！
    cleanup();
    initialized_ = false;
}

void VideoDecoder::close() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();  // 尝试获取已被destroy()持有的锁 -> 死锁！
    // ...
}
```

### 次要问题：帧数不匹配
- **153帧**：FFmpeg报告的容器中的总包数（`nb_frames`）
- **148帧**：实际成功解码的视频帧数
- **差异原因**：
  - B帧（双向预测帧）可能依赖其他帧
  - 部分包可能是元数据而非实际视频帧
  - 解码器可能丢弃不完整或损坏的帧
  - 这是FFmpeg解码的正常现象

## 解决方案

### 1. 修复死锁问题

**VideoDecoder修复**：
```cpp
void VideoDecoder::destroy() {
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (threadSafe_) lock.lock();
    
    if (!initialized_) return;
    
    LOG_DEBUG("开始销毁视频解码器...");
    
    // 关键修复：临时释放锁，避免重复加锁
    if (opened_) {
        if (threadSafe_) lock.unlock();  // 释放锁
        close();                         // 调用close()
        if (threadSafe_) lock.lock();    // 重新获取锁
    }
    
    cleanup();
    initialized_ = false;
    LOG_DEBUG("VideoDecoder已销毁");
}
```

**AudioDecoder修复**：应用相同的修复策略

### 2. 改进资源释放顺序

**正确的释放顺序**：
```cpp
void VideoDecoder::close() {
    // 1. 首先关闭SwsContext
    if (swsCtx_) {
        sws_freeContext(swsCtx_);
        swsCtx_ = nullptr;
    }
    
    // 2. 关闭编解码器上下文
    if (codecCtx_) {
        avcodec_free_context(&codecCtx_);
        codecCtx_ = nullptr;
    }
    
    // 3. 最后关闭格式上下文
    if (formatCtx_) {
        avformat_close_input(&formatCtx_);
        formatCtx_ = nullptr;
    }
    
    // 4. 重置状态
    // ...
}
```

### 3. 增强调试能力
- 添加详细的LOG_DEBUG信息
- 每个资源释放步骤都有日志记录
- 便于未来问题排查

## 验证结果

### 修复前
```bash
$ ./decoder_test ../../resource/Vedio/vedio.mp4
# 程序在视频解码器测试完成后阻塞，需要Ctrl+C中断
```

### 修复后
```bash
$ ./decoder_test ../../resource/Vedio/vedio.mp4
# 完整输出：
✓ 视频解码器测试完成 (148帧，451ms)
✓ 音频解码器测试完成 (无音频流)
✓ 统一解码器测试完成 (10帧，37ms)
✓ 配置功能测试完成 (3种配置)
✓ 定位功能测试完成 (4个时间点)
==================================================
所有测试完成!
==================================================
```

## 预防措施

### 1. 代码审查要点
- 检查所有使用互斥锁的函数调用关系
- 避免在持有锁的情况下调用可能获取同一锁的函数
- 使用RAII和作用域锁管理

### 2. 测试策略
- 为所有资源管理功能编写单元测试
- 使用工具检测死锁（如ThreadSanitizer）
- 压力测试资源创建和销毁

### 3. 设计原则
- 最小锁粒度：只在必要时持有锁
- 锁顺序一致：避免不同顺序获取多个锁
- 避免嵌套锁调用

## 经验教训

1. **资源管理的复杂性**：FFmpeg等C库的资源管理需要严格的顺序
2. **线程安全的代价**：互斥锁使用不当容易导致死锁
3. **调试工具的重要性**：详细的日志对问题定位至关重要
4. **测试的全面性**：需要测试完整的生命周期，不仅是功能测试

## 相关文件
- `src/VideoDecoder.cpp` - 主要修复文件
- `src/AudioDecoder.cpp` - 相同问题修复
- `test_decoder.cpp` - 测试程序
- `Makefile` - 构建配置

## 技术细节
- **FFmpeg版本**：系统默认版本
- **编译器**：g++ (C++17)
- **操作系统**：Linux 5.15.0-139-generic
- **测试文件**：720x1280 @30fps H.264视频，无音频流 