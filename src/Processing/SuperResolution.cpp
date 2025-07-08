#include "SuperResolution.h"
#include "../SuperEigen/include/SuperResEngine.h"
#include "../Utils/Logger.h"
#include <iostream>
#include <filesystem>

SuperResolutionProcessor::SuperResolutionProcessor(const std::string& model_path)
    : model_path_(model_path)
{
    // 使用SuperEigen模块初始化
    engine_ = std::make_unique<SuperEigen::SuperResEngine>();
    
    // 检查模型文件是否存在
    if (!std::filesystem::exists(model_path)) {
        throw std::runtime_error("Model file not found: " + model_path);
    }
    
    // 初始化引擎
    if (!engine_->initialize(model_path)) {
        throw std::runtime_error("Failed to initialize SuperRes engine with model: " + model_path);
    }
    
    LOG_INFO("SuperResolution initialized with model: " + model_path);
}

cv::Mat SuperResolutionProcessor::Process(const cv::Mat& input_bgr) {
    if (input_bgr.empty()) {
        throw std::runtime_error("Input image is empty");
    }
    
    // 直接使用SuperEigen引擎处理图像
    cv::Mat result = engine_->Process(input_bgr);
    
    if (result.empty()) {
        throw std::runtime_error("Super resolution processing failed");
    }
    
    return result;
}

