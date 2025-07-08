#include "include/Decoder.h"
#include "include/VideoDecoder.h"
#include "include/AudioDecoder.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <opencv2/opencv.hpp>

void printUsage(const char* program) {
    std::cout << "用法: " << program << " <video_file> [test_mode]" << std::endl;
    std::cout << "test_mode:" << std::endl;
    std::cout << "  video    - 仅测试视频解码" << std::endl;
    std::cout << "  audio    - 仅测试音频解码" << std::endl;
    std::cout << "  all      - 测试统一解码器 (默认)" << std::endl;
    std::cout << "  config   - 测试配置功能" << std::endl;
    std::cout << "  seek     - 测试定位功能" << std::endl;
}

void testVideoDecoder(const std::string& filepath) {
    std::cout << "\n=== 测试视频解码器 ===" << std::endl;
    
    VideoDecoder decoder;
    
    // 配置解码器
    VideoDecoderConfig config;
    config.outputPixelFormat = "bgr24";
    config.threadCount = 4;
    
    if (!decoder.initialize(config)) {
        std::cerr << "视频解码器初始化失败" << std::endl;
        return;
    }
    
    decoder.setThreadSafe(true);
    
    if (!decoder.open(filepath)) {
        std::cerr << "无法打开视频文件" << std::endl;
        return;
    }
    
    // 显示视频信息
    VideoInfo info = decoder.getVideoInfo();
    std::cout << "视频信息:" << std::endl;
    std::cout << "  分辨率: " << info.width << "x" << info.height << std::endl;
    std::cout << "  帧率: " << info.frameRate << " fps" << std::endl;
    std::cout << "  总帧数: " << info.totalFrames << std::endl;
    std::cout << "  编解码器: " << info.codecName << std::endl;
    
    // 获取所有帧到vector中
    std::cout << "\n获取所有帧到vector..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<FrameData> allFrames = decoder.getAllFrames();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "总共获取 " << allFrames.size() << " 帧，耗时: " << duration.count() << " ms" << std::endl;
    std::cout << "平均每帧: " << (allFrames.size() > 0 ? duration.count() / (double)allFrames.size() : 0) << " ms" << std::endl;
    
    // 输出前20帧的详细信息
    std::cout << "\n前20帧详细信息:" << std::endl;
    std::cout << "帧号 | 宽度x高度 | PTS | 时间戳(秒) | 帧索引 | 源标签 | 图像类型" << std::endl;
    std::cout << "-----|-----------|-----|------------|--------|--------|----------" << std::endl;
    
    int displayCount = std::min(20, (int)allFrames.size());
    for (int i = 0; i < displayCount; i++) {
        const auto& frame = allFrames[i];
        std::cout << std::setw(4) << i << " | "
                  << std::setw(9) << (std::to_string(frame.width) + "x" + std::to_string(frame.height)) << " | "
                  << std::setw(7) << frame.pts << " | "
                  << std::fixed << std::setprecision(6) << std::setw(10) << frame.timestamp << " | "
                  << std::setw(6) << frame.frameIndex << " | "
                  << std::setw(6) << frame.sourceTag << " | "
                  << frame.image.type() << "(" << frame.image.channels() << "ch)" << std::endl;
    }
    
    decoder.close();
}

void testAudioDecoder(const std::string& filepath) {
    std::cout << "\n=== 测试音频解码器 ===" << std::endl;
    
    AudioDecoder decoder;
    
    // 配置解码器
    AudioDecoderConfig config;
    config.outputSampleRate = 44100;
    config.outputChannels = 2;
    config.outputFormat = AV_SAMPLE_FMT_S16;
    
    if (!decoder.initialize(config)) {
        std::cerr << "音频解码器初始化失败" << std::endl;
        return;
    }
    
    decoder.setThreadSafe(true);
    
    if (!decoder.open(filepath)) {
        std::cerr << "无法打开音频文件" << std::endl;
        return;
    }
    
    // 显示音频信息
    AudioInfo info = decoder.getAudioInfo();
    std::cout << "音频信息:" << std::endl;
    std::cout << "  采样率: " << info.sampleRate << " Hz" << std::endl;
    std::cout << "  声道数: " << info.channels << std::endl;
    std::cout << "  总样本数: " << info.totalSamples << std::endl;
    std::cout << "  编解码器: " << info.codecName << std::endl;
    std::cout << "  比特率: " << info.bitRate << " bps" << std::endl;
    
    // 测试单帧读取
    std::cout << "\n测试单帧读取..." << std::endl;
    AudioFrameData frame;
    int frameCount = 0;
    size_t totalSamples = 0;
    auto start = std::chrono::high_resolution_clock::now();
    
    while (decoder.readNextFrame(frame) && frameCount < 10) {
        std::cout << "帧 " << frameCount << ": " 
                  << frame.nbSamples << " 样本, " 
                  << frame.data.size() << " 字节, "
                  << "时间戳: " << frame.timestamp << "s" << std::endl;
        totalSamples += frame.nbSamples;
        frameCount++;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "读取 " << frameCount << " 帧 (" << totalSamples << " 样本) 耗时: " 
              << duration.count() << " ms" << std::endl;
    
    decoder.close();
}

void testUnifiedDecoder(const std::string& filepath) {
    std::cout << "\n=== 测试统一解码器 ===" << std::endl;
    
    Decoder decoder;
    
    // 配置解码器
    DecoderConfig config;
    config.enableVideo = true;
    config.enableAudio = true;
    config.threadSafe = true;
    
    // 视频配置
    config.videoConfig.outputPixelFormat = "bgr24";
    config.videoConfig.threadCount = 4;
    
    // 音频配置
    config.audioConfig.outputSampleRate = 44100;
    config.audioConfig.outputChannels = 2;
    
    if (!decoder.initialize(config)) {
        std::cerr << "统一解码器初始化失败" << std::endl;
        return;
    }
    
    if (!decoder.open(filepath)) {
        std::cerr << "无法打开媒体文件" << std::endl;
        return;
    }
    
    // 显示媒体信息
    MediaInfo info = decoder.getMediaInfo();
    std::cout << "媒体信息:" << std::endl;
    std::cout << "  文件名: " << info.filename << std::endl;
    std::cout << "  文件大小: " << info.fileSize << " 字节" << std::endl;
    std::cout << "  总时长: " << info.duration << " 秒" << std::endl;
    std::cout << "  视频流: " << (info.hasVideo ? "有" : "无") << std::endl;
    std::cout << "  音频流: " << (info.hasAudio ? "有" : "无") << std::endl;
    
    if (info.hasVideo) {
        std::cout << "  视频分辨率: " << info.videoInfo.width << "x" << info.videoInfo.height << std::endl;
        std::cout << "  视频帧率: " << info.videoInfo.frameRate << " fps" << std::endl;
    }
    
    if (info.hasAudio) {
        std::cout << "  音频采样率: " << info.audioInfo.sampleRate << " Hz" << std::endl;
        std::cout << "  音频声道数: " << info.audioInfo.channels << std::endl;
    }
    
    // 测试单帧读取
    std::cout << "\n测试混合读取..." << std::endl;
    FrameData videoFrame;
    AudioFrameData audioFrame;
    int videoCount = 0, audioCount = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 20; i++) {
        if (decoder.hasVideo() && decoder.readNextVideoFrame(videoFrame)) {
            if (videoCount < 5) {
                std::cout << "视频帧 " << videoCount << ": " << videoFrame.timestamp << "s" << std::endl;
            }
            videoCount++;
        }
        
        if (decoder.hasAudio() && decoder.readNextAudioFrame(audioFrame)) {
            if (audioCount < 5) {
                std::cout << "音频帧 " << audioCount << ": " << audioFrame.timestamp << "s" << std::endl;
            }
            audioCount++;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "读取 " << videoCount << " 视频帧, " << audioCount << " 音频帧, 耗时: " 
              << duration.count() << " ms" << std::endl;
    
    decoder.close();
}

void testConfiguration(const std::string& filepath) {
    std::cout << "\n=== 测试配置功能 ===" << std::endl;
    
    VideoDecoder decoder;
    
    // 测试不同配置
    std::vector<VideoDecoderConfig> configs = {
        {.outputPixelFormat = "bgr24", .threadCount = 1},
        {.outputPixelFormat = "rgb24", .threadCount = 4},
        {.outputPixelFormat = "gray", .maxWidth = 640, .maxHeight = 480}
    };
    
    for (size_t i = 0; i < configs.size(); i++) {
        std::cout << "\n配置 " << (i+1) << ":" << std::endl;
        std::cout << "  像素格式: " << configs[i].outputPixelFormat << std::endl;
        std::cout << "  线程数: " << configs[i].threadCount << std::endl;
        std::cout << "  最大尺寸: " << configs[i].maxWidth << "x" << configs[i].maxHeight << std::endl;
        
        if (!decoder.initialize(configs[i])) {
            std::cerr << "配置 " << (i+1) << " 初始化失败" << std::endl;
            continue;
        }
        
        if (!decoder.open(filepath)) {
            std::cerr << "配置 " << (i+1) << " 打开文件失败" << std::endl;
            decoder.destroy();
            continue;
        }
        
        FrameData frame;
        if (decoder.readNextFrame(frame)) {
            std::cout << "  解码成功: " << frame.width << "x" << frame.height 
                      << ", 通道数: " << frame.image.channels() << std::endl;
        }
        
        decoder.close();
        decoder.destroy();
    }
}

void testSeeking(const std::string& filepath) {
    std::cout << "\n=== 测试定位功能 ===" << std::endl;
    
    Decoder decoder;
    
    if (!decoder.initialize()) {
        std::cerr << "解码器初始化失败" << std::endl;
        return;
    }
    
    if (!decoder.open(filepath)) {
        std::cerr << "无法打开文件" << std::endl;
        return;
    }
    
    MediaInfo info = decoder.getMediaInfo();
    
    // 测试时间定位
    std::vector<double> seekTimes = {0.0, info.duration * 0.25, info.duration * 0.5, info.duration * 0.75};
    
    for (double time : seekTimes) {
        std::cout << "\n定位到 " << time << " 秒..." << std::endl;
        
        if (decoder.seekToTime(time)) {
            FrameData videoFrame;
            AudioFrameData audioFrame;
            
            if (decoder.hasVideo() && decoder.readNextVideoFrame(videoFrame)) {
                std::cout << "  视频帧时间戳: " << videoFrame.timestamp << "s" << std::endl;
            }
            
            if (decoder.hasAudio() && decoder.readNextAudioFrame(audioFrame)) {
                std::cout << "  音频帧时间戳: " << audioFrame.timestamp << "s" << std::endl;
            }
        } else {
            std::cout << "  定位失败" << std::endl;
        }
    }
    
    decoder.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string filepath = argv[1];
    std::string mode = (argc > 2) ? argv[2] : "all";
    
    std::cout << "VideoSR-Lite 解码器测试程序" << std::endl;
    std::cout << "文件: " << filepath << std::endl;
    std::cout << "模式: " << mode << std::endl;
    
    
    
    return 0;
} 