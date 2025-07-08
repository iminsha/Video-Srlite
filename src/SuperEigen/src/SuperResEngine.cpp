#include "../include/SuperResEngine.h"
#include "../../Utils/Logger.h"
#include <chrono>
#include <iostream>
#include <filesystem>

namespace SuperEigen {

SuperResEngine::SuperResEngine()
    : initialized_(false)
    , processedFrames_(0)
    , totalProcessTime_(0.0)
    , lastProcessTime_(0.0) {
}

SuperResEngine::~SuperResEngine() {
    // 自动清理资源
}

bool SuperResEngine::initializeDefault() {
    std::string defaultModelPath = getDefaultModelPath();
    return initialize(defaultModelPath, false, 0);
}

bool SuperResEngine::initialize(const std::string& modelPath, bool useGPU, int gpuId) {
    try {
        // 确定模型路径
        std::string actualModelPath = modelPath.empty() ? getDefaultModelPath() : modelPath;
        
        if (!std::filesystem::exists(actualModelPath)) {
            LOG_ERROR("Model file not found: " + actualModelPath);
            return false;
        }
        
        // 创建配置
        config_.modelPath = actualModelPath;
        config_.device = useGPU ? SuperResConfig::GPU : SuperResConfig::CPU;
        config_.deviceId = gpuId;
        
        // 从模型名推断倍率
        std::string filename = std::filesystem::path(actualModelPath).filename().string();
        if (filename.find("x4") != std::string::npos) {
            config_.scaleFactor = 4;
        } else {
            config_.scaleFactor = 2;
        }
        
        // 创建会话
        session_ = std::make_unique<ModelSession>(config_);
        if (!session_->initialize(actualModelPath)) {
            LOG_ERROR("Failed to initialize model session");
            return false;
        }
        
        // 创建处理器
        processor_ = std::make_unique<PrePostProcessor>(config_);
        
        initialized_ = true;
        
        LOG_INFO("SuperResEngine initialized successfully");
        LOG_INFO("Model: " + actualModelPath);
        LOG_INFO("Scale: " + std::to_string(config_.scaleFactor) + "x");
        LOG_INFO("Device: " + std::string(useGPU ? "GPU" : "CPU"));
        
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Initialization failed: " + std::string(e.what()));
        return false;
    }
}

cv::Mat SuperResEngine::Process(const cv::Mat& input_bgr) {
    if (!initialized_) {
        LOG_ERROR("SuperResEngine not initialized");
        return input_bgr.clone();
    }
    
    try {
        return processImageInternal(input_bgr);
    } catch (const std::exception& e) {
        LOG_ERROR("Processing error: " + std::string(e.what()));
        return input_bgr.clone();
    }
}

FrameData SuperResEngine::processFrame(const FrameData& input) {
    if (!initialized_) {
        throw std::runtime_error("SuperResEngine not initialized");
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    try {
        // 处理图像
        cv::Mat processedImage = processImageInternal(input.image);
        
        // 创建输出帧
        FrameData output = input;  // 复制所有元数据
        output.image = processedImage;
        updateFrameMetadata(output);
        
        // 统计
        auto endTime = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        collectStats(timeMs);
        
        return output;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Frame processing error: " + std::string(e.what()));
        throw;
    }
}

std::vector<FrameData> SuperResEngine::processBatch(const std::vector<FrameData>& inputs) {
    if (!initialized_) {
        throw std::runtime_error("SuperResEngine not initialized");
    }
    
    if (inputs.empty()) {
        return {};
    }
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    try {
        std::vector<FrameData> outputs;
        outputs.reserve(inputs.size());
        
        for (size_t i = 0; i < inputs.size(); ++i) {
            // 处理单帧
            cv::Mat processedImage = processImageInternal(inputs[i].image);
            
            // 创建输出帧
            FrameData output = inputs[i];
            output.image = processedImage;
            updateFrameMetadata(output);
            outputs.push_back(output);
            
            // 进度回调
            if (progressCallback_) {
                progressCallback_(i + 1, inputs.size());
            }
        }
        
        // 统计
        auto endTime = std::chrono::high_resolution_clock::now();
        double timeMs = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        collectStats(timeMs);
        
        return outputs;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Batch processing error: " + std::string(e.what()));
        throw;
    }
}

SuperResEngine::ProcessingStats SuperResEngine::getStats() const {
    std::lock_guard<std::mutex> lock(statsMutex_);
    
    ProcessingStats stats;
    stats.totalFrames = processedFrames_;
    stats.avgTimeMs = processedFrames_ > 0 ? totalProcessTime_ / processedFrames_ : 0.0;
    stats.lastTimeMs = lastProcessTime_;
    stats.modelPath = config_.modelPath;
    stats.scaleFactor = config_.scaleFactor;
    stats.useGPU = config_.device == SuperResConfig::GPU;
    
    return stats;
}

bool SuperResEngine::switchModel(const std::string& modelPath) {
    if (!std::filesystem::exists(modelPath)) {
        LOG_ERROR("Model file not found: " + modelPath);
        return false;
    }
    
    // 重新初始化
    bool useGPU = config_.device == SuperResConfig::GPU;
    int gpuId = config_.deviceId;
    
    initialized_ = false;
    session_.reset();
    processor_.reset();
    
    return initialize(modelPath, useGPU, gpuId);
}

bool SuperResEngine::switchDevice(bool useGPU, int gpuId) {
    if (!initialized_) {
        config_.device = useGPU ? SuperResConfig::GPU : SuperResConfig::CPU;
        config_.deviceId = gpuId;
        return true;
    }
    
    // 重新初始化
    std::string modelPath = config_.modelPath;
    
    initialized_ = false;
    session_.reset();
    processor_.reset();
    
    return initialize(modelPath, useGPU, gpuId);
}

void SuperResEngine::setProgressCallback(std::function<void(int, int)> callback) {
    progressCallback_ = callback;
}

std::string SuperResEngine::getDefaultModelPath() {
    // 从SuperEigen目录向上查找模型文件
    std::filesystem::path modelPath = "../../onnx/RealESRGAN_x2plus.fp16.onnx";
    
    if (!std::filesystem::exists(modelPath)) {
        // 尝试其他可能的路径
        std::vector<std::string> possiblePaths = {
            "../../onnx/RealESRGAN_x2plus.fp16.onnx",
            "../../../onnx/RealESRGAN_x2plus.fp16.onnx",
            "VideoSR-Lite/onnx/RealESRGAN_x2plus.fp16.onnx",
            "onnx/RealESRGAN_x2plus.fp16.onnx"
        };
        
        for (const auto& path : possiblePaths) {
            if (std::filesystem::exists(path)) {
                modelPath = path;
                break;
            }
        }
    }
    
    return modelPath.string();
}

bool SuperResEngine::isGPUSupported() {
    return SuperResConfig::checkGPUAvailable();
}

std::vector<std::string> SuperResEngine::getAvailableModels() {
    std::vector<std::string> models;
    
    // 从SuperEigen目录向上查找onnx目录
    std::filesystem::path onnxDir = "../../onnx";
    
    if (!std::filesystem::exists(onnxDir)) {
        // 尝试其他可能的路径
        std::vector<std::string> possiblePaths = {
            "../../onnx",
            "../../../onnx", 
            "VideoSR-Lite/onnx",
            "onnx"
        };
        
        for (const auto& path : possiblePaths) {
            if (std::filesystem::exists(path)) {
                onnxDir = path;
                break;
            }
        }
    }
    
    if (!std::filesystem::exists(onnxDir)) {
        return models;
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(onnxDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".onnx") {
            models.push_back(entry.path().string());
        }
    }
    
    std::sort(models.begin(), models.end());
    return models;
}

// 私有方法实现

void SuperResEngine::updateFrameMetadata(FrameData& frame) {
    frame.width *= config_.scaleFactor;
    frame.height *= config_.scaleFactor;
    
    // 更新源标签
    if (!frame.sourceTag.empty()) {
        frame.sourceTag += "_SR" + std::to_string(config_.scaleFactor) + "x";
    } else {
        frame.sourceTag = "SR" + std::to_string(config_.scaleFactor) + "x";
    }
}

void SuperResEngine::collectStats(double timeMs) {
    std::lock_guard<std::mutex> lock(statsMutex_);
    processedFrames_++;
    totalProcessTime_ += timeMs;
    lastProcessTime_ = timeMs;
}

cv::Mat SuperResEngine::processImageInternal(const cv::Mat& image) {
    // 检查输入图像尺寸，如果太大则分块处理或缩放处理
    int maxSafeSize = 512; // CPU安全处理的最大尺寸
    
    if (image.cols > maxSafeSize || image.rows > maxSafeSize) {
        // 图像太大，需要分块处理或缩放处理
        // 这里先实现缩放处理，保持完整性
        double scale = std::min(double(maxSafeSize) / image.cols, double(maxSafeSize) / image.rows);
        
        // 确保尺寸能被4整除
        int newWidth = ((int)(image.cols * scale) / 4) * 4;
        int newHeight = ((int)(image.rows * scale) / 4) * 4;
        
        cv::Mat resizedInput;
        cv::resize(image, resizedInput, cv::Size(newWidth, newHeight), 0, 0, cv::INTER_LANCZOS4);
        
        // 处理缩放后的图像
        Ort::Value inputTensor = processor_->preprocess(resizedInput, session_->getMemoryInfo());
        Ort::Value outputTensor = session_->inference(inputTensor);
        cv::Mat resizedOutput = processor_->postprocess(outputTensor, resizedInput.size());
        
        // 将结果缩放回原始比例
        cv::Mat finalOutput;
        int finalWidth = image.cols * config_.scaleFactor;
        int finalHeight = image.rows * config_.scaleFactor;
        cv::resize(resizedOutput, finalOutput, cv::Size(finalWidth, finalHeight), 0, 0, cv::INTER_LANCZOS4);
        
        return finalOutput;
    } else {
        // 图像尺寸安全，直接处理
        // 但仍需确保尺寸能被4整除
        cv::Mat processImage = image;
        if (image.cols % 4 != 0 || image.rows % 4 != 0) {
            int adjustedWidth = (image.cols / 4) * 4;
            int adjustedHeight = (image.rows / 4) * 4;
            cv::resize(image, processImage, cv::Size(adjustedWidth, adjustedHeight));
        }
        
        // 预处理
        Ort::Value inputTensor = processor_->preprocess(processImage, session_->getMemoryInfo());
        
        // 推理
        Ort::Value outputTensor = session_->inference(inputTensor);
        
        // 后处理
        return processor_->postprocess(outputTensor, processImage.size());
    }
}

} // namespace SuperEigen 