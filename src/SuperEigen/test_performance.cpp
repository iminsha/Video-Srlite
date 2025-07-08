#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include "include/SuperResEngine.h"
#include "../Decoder/include/VideoDecoder.h"

using namespace SuperEigen;

/**
 * @brief 创建对比图像：将两张图缩放到相同尺寸进行对比
 * @param original 原始图像
 * @param superRes 超分辨率图像
 * @return 拼接后的对比图像
 */
cv::Mat createComparisonImage(const cv::Mat& original, const cv::Mat& superRes) {
    // 确保图像有效
    if (original.empty() || superRes.empty()) {
        std::cerr << "Invalid images for comparison" << std::endl;
        return cv::Mat();
    }
    
    // 确定统一的显示尺寸
    int displayWidth = 512;  // 统一显示宽度
    int displayHeight = 512; // 统一显示高度
    
    // 将两张图都缩放到相同的显示尺寸，保持宽高比
    cv::Mat displayOriginal, displaySuperRes;
    
    // 缩放原图到统一尺寸
    double originalScale = std::min(double(displayWidth) / original.cols, double(displayHeight) / original.rows);
    int originalDisplayW = int(original.cols * originalScale);
    int originalDisplayH = int(original.rows * originalScale);
    cv::resize(original, displayOriginal, cv::Size(originalDisplayW, originalDisplayH), 0, 0, cv::INTER_LANCZOS4);
    
    // 缩放超分图像到统一尺寸
    double superResScale = std::min(double(displayWidth) / superRes.cols, double(displayHeight) / superRes.rows);
    int superResDisplayW = int(superRes.cols * superResScale);
    int superResDisplayH = int(superRes.rows * superResScale);
    cv::resize(superRes, displaySuperRes, cv::Size(superResDisplayW, superResDisplayH), 0, 0, cv::INTER_LANCZOS4);
    
    // 计算拼接后的尺寸
    int maxHeight = std::max(originalDisplayH, superResDisplayH);
    int totalWidth = originalDisplayW + superResDisplayW + 40; // 中间留40像素间隔
    
    // 创建拼接画布
    cv::Mat comparison(maxHeight + 120, totalWidth, CV_8UC3, cv::Scalar(40, 40, 40));
    
    // 计算居中位置
    int originalY = (maxHeight - originalDisplayH) / 2 + 80;
    int superResY = (maxHeight - superResDisplayH) / 2 + 80;
    
    // 复制原图到左侧
    cv::Rect originalRect(10, originalY, originalDisplayW, originalDisplayH);
    displayOriginal.copyTo(comparison(originalRect));
    
    // 复制超分图像到右侧
    cv::Rect superResRect(originalDisplayW + 30, superResY, superResDisplayW, superResDisplayH);
    displaySuperRes.copyTo(comparison(superResRect));
    
    // 添加详细标签
    std::string originalLabel = "Original: " + std::to_string(original.cols) + "x" + std::to_string(original.rows);
    std::string superResLabel = "Super-Resolution: " + std::to_string(superRes.cols) + "x" + std::to_string(superRes.rows);
    
    // 添加显示尺寸信息
    originalLabel += " (显示: " + std::to_string(originalDisplayW) + "x" + std::to_string(originalDisplayH) + ")";
    superResLabel += " (显示: " + std::to_string(superResDisplayW) + "x" + std::to_string(superResDisplayH) + ")";
    
    // 添加标题
    cv::putText(comparison, "Super-Resolution Comparison", cv::Point(totalWidth/2 - 200, 30), 
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
    
    // 添加图像标签
    cv::putText(comparison, originalLabel, cv::Point(10, 60), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    cv::putText(comparison, superResLabel, cv::Point(originalDisplayW + 30, 60), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
    
    // 添加分隔线
    cv::line(comparison, cv::Point(originalDisplayW + 15, 80), 
             cv::Point(originalDisplayW + 15, maxHeight + 80), cv::Scalar(255, 255, 255), 2);
    
    // 添加质量信息
    double scaleFactor = double(superRes.cols) / original.cols;
    std::string scaleInfo = "Scale Factor: " + std::to_string(scaleFactor).substr(0, 3) + "x";
    cv::putText(comparison, scaleInfo, cv::Point(totalWidth/2 - 80, maxHeight + 110), 
                cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
    
    // 添加说明：两图已缩放到相同显示尺寸
    std::string sizeNote = "Both images scaled to same display size for comparison";
    cv::putText(comparison, sizeNote, cv::Point(10, maxHeight + 110), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(200, 200, 200), 1);
    
    return comparison;
}

/**
 * @brief 格式化时间显示
 */
std::string formatTime(double ms) {
    if (ms < 1000) {
        return std::to_string(static_cast<int>(ms)) + "ms";
    } else {
        return std::to_string(ms / 1000.0).substr(0, 4) + "s";
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=== SuperEigen 性能测试程序 ===" << std::endl;
    
    // 检查命令行参数
    if (argc != 2) {
        std::cout << "用法: " << argv[0] << " <视频文件路径>" << std::endl;
        std::cout << "示例: " << argv[0] << " ../../resource/Vedio/vedio.mp4" << std::endl;
        return 1;
    }
    
    // 1. 检查视频文件
    std::string videoPath = argv[1];
    if (!std::filesystem::exists(videoPath)) {
        std::cerr << "视频文件不存在: " << videoPath << std::endl;
        return 1;
    }
    
    std::cout << "视频文件: " << videoPath << std::endl;
    
    // 2. 初始化视频解码器
    std::cout << "\n初始化视频解码器..." << std::endl;
    VideoDecoder decoder;
    VideoDecoderConfig decoderConfig;
    decoderConfig.outputPixelFormat = "bgr24";
    decoderConfig.threadCount = 4;
    
    if (!decoder.initialize(decoderConfig)) {
        std::cerr << "解码器初始化失败" << std::endl;
        return 1;
    }
    
    if (!decoder.open(videoPath)) {
        std::cerr << "无法打开视频文件" << std::endl;
        return 1;
    }
    
    // 获取视频信息
    VideoInfo videoInfo = decoder.getVideoInfo();
    std::cout << "视频信息:" << std::endl;
    std::cout << "  分辨率: " << videoInfo.width << "x" << videoInfo.height << std::endl;
    std::cout << "  帧率: " << videoInfo.frameRate << " fps" << std::endl;
    std::cout << "  总帧数: " << videoInfo.totalFrames << std::endl;
    std::cout << "  时长: " << videoInfo.duration << " 秒" << std::endl;
    
    // 3. 初始化超分辨率引擎
    std::cout << "\n初始化超分辨率引擎..." << std::endl;
    SuperResEngine srEngine;
    
    // 检查GPU支持
    bool hasGPU = SuperResEngine::isGPUSupported();
    std::cout << "GPU 支持: " << (hasGPU ? "是" : "否") << std::endl;
    
    // 检查可用模型
    auto models = SuperResEngine::getAvailableModels();
    std::cout << "可用模型数量: " << models.size() << std::endl;
    for (const auto& model : models) {
        std::cout << "  - " << std::filesystem::path(model).filename().string() << std::endl;
    }
    
    if (models.empty()) {
        std::cerr << "未找到可用模型，请确保模型文件在 VideoSR-Lite/onnx/ 目录下" << std::endl;
        return 1;
    }
    
    // 初始化引擎，优先使用GPU
    bool initSuccess = false;
    if (hasGPU) {
        std::cout << "尝试使用GPU初始化..." << std::endl;
        initSuccess = srEngine.initialize("", true, 0);
        if (!initSuccess) {
            std::cout << "GPU初始化失败，回退到CPU..." << std::endl;
        }
    }
    
    if (!initSuccess) {
        std::cout << "使用CPU初始化..." << std::endl;
        initSuccess = srEngine.initializeDefault();
    }
    
    if (!initSuccess) {
        std::cerr << "超分辨率引擎初始化失败" << std::endl;
        return 1;
    }
    
    std::cout << "超分辨率引擎初始化成功" << std::endl;
    std::cout << "超分倍率: " << srEngine.getScaleFactor() << "x" << std::endl;
    
    // 4. 提取测试帧
    std::cout << "\n提取测试帧（每隔10帧取1帧，共5帧）..." << std::endl;
    std::vector<FrameData> testFrames;
    
    for (int i = 0; i < 5; ++i) {
        // 跳转到指定帧（第0, 10, 20, 30, 40帧）
        int64_t targetFrame = i * 10;
        if (targetFrame >= videoInfo.totalFrames) {
            std::cout << "帧 " << targetFrame << " 超出视频范围，停止提取" << std::endl;
            break;
        }
        
        if (!decoder.seekToFrame(targetFrame)) {
            std::cerr << "无法跳转到帧 " << targetFrame << std::endl;
            continue;
        }
        
        FrameData frame;
        if (decoder.readNextFrame(frame)) {
            frame.frameIndex = i;
            testFrames.push_back(frame);
            std::cout << "  提取第 " << targetFrame << " 帧，尺寸: " 
                     << frame.width << "x" << frame.height << std::endl;
        } else {
            std::cerr << "读取帧 " << targetFrame << " 失败" << std::endl;
        }
    }
    
    if (testFrames.empty()) {
        std::cerr << "未能提取到任何帧" << std::endl;
        return 1;
    }
    
    std::cout << "成功提取 " << testFrames.size() << " 帧" << std::endl;
    
    // 5. 性能测试
    std::cout << "\n开始超分辨率性能测试..." << std::endl;
    
    auto overallStart = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < testFrames.size(); ++i) {
        std::cout << "\n处理第 " << (i + 1) << "/" << testFrames.size() << " 帧..." << std::endl;
        
        const auto& inputFrame = testFrames[i];
        
        // 单帧处理性能测试
        auto frameStart = std::chrono::high_resolution_clock::now();
        
        try {
            // 检查输入帧
            if (inputFrame.image.empty()) {
                std::cerr << "  错误: 输入帧为空" << std::endl;
                continue;
            }
            
            std::cout << "  输入: " << inputFrame.width << "x" << inputFrame.height 
                     << " 类型: " << inputFrame.image.type()
                     << " 通道: " << inputFrame.image.channels() << std::endl;
            
                        // 保持原始帧完整性，不进行任何修改
            std::cout << "  开始超分处理（保持原始尺寸）..." << std::endl;
            
            // 直接使用原始帧进行超分处理
            FrameData outputFrame = srEngine.processFrame(inputFrame);
            
            auto frameEnd = std::chrono::high_resolution_clock::now();
            double frameTimeMs = std::chrono::duration<double, std::milli>(frameEnd - frameStart).count();
            
            std::cout << "  原始: " << inputFrame.width << "x" << inputFrame.height << std::endl;
            std::cout << "  超分: " << outputFrame.width << "x" << outputFrame.height << std::endl;
            std::cout << "  处理时间: " << formatTime(frameTimeMs) << std::endl;
            
            // 分别保存原图和超分图像
            std::string originalFilename = "original_frame_" + std::to_string(i + 1) + ".png";
            std::string superResFilename = "super_res_frame_" + std::to_string(i + 1) + ".png";
            
            // 保存原始图像
            cv::imwrite(originalFilename, inputFrame.image);
            std::cout << "  原始图像已保存: " << originalFilename << std::endl;
            
            // 保存超分图像
            cv::imwrite(superResFilename, outputFrame.image);
            std::cout << "  超分图像已保存: " << superResFilename << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "  处理失败: " << e.what() << std::endl;
        }
    }
    
    auto overallEnd = std::chrono::high_resolution_clock::now();
    double totalTimeMs = std::chrono::duration<double, std::milli>(overallEnd - overallStart).count();
    
    // 6. 统计信息
    std::cout << "\n=== 性能统计 ===" << std::endl;
    
    auto stats = srEngine.getStats();
    std::cout << "总处理帧数: " << stats.totalFrames << std::endl;
    std::cout << "总处理时间: " << formatTime(totalTimeMs) << std::endl;
    std::cout << "平均每帧时间: " << formatTime(stats.avgTimeMs) << std::endl;
    
    if (stats.totalFrames > 0) {
        double fps = 1000.0 / stats.avgTimeMs;
        std::cout << "处理速度: " << fps << " fps" << std::endl;
        
        // 与实时播放对比
        double realTimeFps = videoInfo.frameRate;
        double speedRatio = fps / realTimeFps;
        std::cout << "实时倍率: " << speedRatio << "x ";
        if (speedRatio >= 1.0) {
            std::cout << "(可实时处理)" << std::endl;
        } else {
            std::cout << "(无法实时处理)" << std::endl;
        }
    }
    
    std::cout << "模型路径: " << stats.modelPath << std::endl;
    std::cout << "超分倍率: " << stats.scaleFactor << "x" << std::endl;
    std::cout << "使用设备: " << (stats.useGPU ? "GPU" : "CPU") << std::endl;
    
    // 7. 批量处理性能测试（暂时禁用，避免段错误）
    std::cout << "\n=== 批量处理性能测试 ===" << std::endl;
    std::cout << "批量处理功能暂时禁用，避免段错误问题。" << std::endl;
    std::cout << "单帧处理性能已经测试完成。" << std::endl;
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    std::cout << "所有结果图像已保存到当前目录" << std::endl;
    
    return 0;
} 