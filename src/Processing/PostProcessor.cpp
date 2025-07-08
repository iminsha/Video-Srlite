#include "PostProcessor.h"
#include <iostream>

PostProcessor::PostProcessor() {
    std::cout << "PostProcessor initialized (stub implementation)" << std::endl;
}

PostProcessor::~PostProcessor() = default;

cv::Mat PostProcessor::sharpen(const cv::Mat& input, float strength) {
    std::cout << "PostProcessor::sharpen called with strength: " << strength << std::endl;
    // 简化实现：直接返回原图
    return input.clone();
}

cv::Mat PostProcessor::adjustContrast(const cv::Mat& input, float contrast) {
    std::cout << "PostProcessor::adjustContrast called with contrast: " << contrast << std::endl;
    // 简化实现：直接返回原图
    return input.clone();
} 