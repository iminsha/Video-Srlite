#include "../include/PrePostProcessor.h"
#include <algorithm>
#include <cstring>

namespace SuperEigen {

PrePostProcessor::PrePostProcessor(const SuperResConfig& config) 
    : config_(config) {
    // 预分配缓冲区以提高性能
    preprocessBuffer_.reserve(1024 * 1024 * 3 * 4); // 预留4MB
    postprocessBuffer_.reserve(1024 * 1024 * 3 * 4);
}

Ort::Value PrePostProcessor::preprocess(const cv::Mat& input, const Ort::MemoryInfo& memoryInfo) {
    // 确保输入是8位BGR图像
    cv::Mat input8u;
    if (input.depth() != CV_8U) {
        input.convertTo(input8u, CV_8U);
    } else {
        input8u = input;
    }

    // BGR -> RGB
    cv::Mat rgb = bgrToRgb(input8u);
    
    // HWC -> CHW 并归一化
    std::vector<float> tensorData = hwcToChw(rgb);
    
    // 应用归一化
    if (config_.inputMean != 0.0f || config_.inputStd != 1.0f) {
        for (auto& val : tensorData) {
            val = (val - config_.inputMean) / config_.inputStd;
        }
    }
    
    // 创建ONNX张量
    std::vector<int64_t> inputShape = {1, 3, input.rows, input.cols}; // NCHW
    
    return Ort::Value::CreateTensor<float>(
        memoryInfo,
        tensorData.data(),
        tensorData.size(),
        inputShape.data(),
        inputShape.size()
    );
}

cv::Mat PrePostProcessor::postprocess(const Ort::Value& outputTensor, const cv::Size& originalSize) {
    // 获取输出张量信息
    auto tensorInfo = outputTensor.GetTensorTypeAndShapeInfo();
    auto shape = tensorInfo.GetShape();
    
    // 假设输出格式为 NCHW
    int64_t channels = shape[1];
    int64_t height = shape[2];
    int64_t width = shape[3];
    
    // 获取张量数据
    const float* tensorData = outputTensor.GetTensorData<float>();
    size_t totalElements = channels * height * width;
    
    // 复制数据到向量
    std::vector<float> outputData(tensorData, tensorData + totalElements);
    
    // 反归一化（如果需要）
    if (config_.inputMean != 0.0f || config_.inputStd != 1.0f) {
        for (auto& val : outputData) {
            val = val * config_.inputStd + config_.inputMean;
        }
    }
    
    // 检查实际输出范围，用于归一化
    auto minmax = std::minmax_element(outputData.begin(), outputData.end());
    float minVal = *minmax.first;
    float maxVal = *minmax.second;
    
    // 根据实际范围进行归一化
    if (maxVal > 1.0f || minVal < 0.0f) {
        // 如果输出不在[0,1]范围，先归一化到[0,1]
        float range = maxVal - minVal;
        if (range > 0) {
            for (auto& val : outputData) {
                val = (val - minVal) / range;
            }
        }
    }
    
    // 确保在[0,1]范围内
    for (auto& val : outputData) {
        val = std::max(0.0f, std::min(1.0f, val));
    }
    
    // CHW -> HWC
    cv::Mat result = chwToHwc(outputData, channels, height, width);
    
    // RGB -> BGR
    result = rgbToBgr(result);
    
    // 转换回8位，确保范围正确
    cv::Mat result8u;
    result.convertTo(result8u, CV_8U, 255.0);
    
    return result8u;
}

std::vector<int64_t> PrePostProcessor::getInputShape(const cv::Mat& image) const {
    return {1, 3, image.rows, image.cols};
}

std::vector<int64_t> PrePostProcessor::getOutputShape(const cv::Mat& image) const {
    int outputHeight = image.rows * config_.scaleFactor;
    int outputWidth = image.cols * config_.scaleFactor;
    return {1, 3, outputHeight, outputWidth};
}

// 私有辅助方法实现

cv::Mat PrePostProcessor::bgrToRgb(const cv::Mat& bgr) {
    cv::Mat rgb;
    cv::cvtColor(bgr, rgb, cv::COLOR_BGR2RGB);
    return rgb;
}

cv::Mat PrePostProcessor::rgbToBgr(const cv::Mat& rgb) {
    cv::Mat bgr;
    cv::cvtColor(rgb, bgr, cv::COLOR_RGB2BGR);
    return bgr;
}

std::vector<float> PrePostProcessor::hwcToChw(const cv::Mat& image) {
    // 确保是float类型
    cv::Mat floatImage;
    if (image.type() != CV_32FC3) {
        image.convertTo(floatImage, CV_32FC3, 1.0 / 255.0);
    } else {
        floatImage = image;
    }
    
    int h = floatImage.rows;
    int w = floatImage.cols;
    int c = floatImage.channels();
    
    std::vector<float> chw(h * w * c);
    
    // HWC -> CHW转换
    std::vector<cv::Mat> channels;
    cv::split(floatImage, channels);
    
    for (int i = 0; i < c; ++i) {
        std::memcpy(chw.data() + i * h * w, 
                   channels[i].data, 
                   h * w * sizeof(float));
    }
    
    return chw;
}

cv::Mat PrePostProcessor::chwToHwc(const std::vector<float>& tensor, 
                                  int channels, int height, int width) {
    std::vector<cv::Mat> channelMats;
    channelMats.reserve(channels);
    
    size_t channelSize = height * width;
    
    for (int c = 0; c < channels; ++c) {
        cv::Mat channel(height, width, CV_32FC1);
        std::memcpy(channel.data, 
                   tensor.data() + c * channelSize, 
                   channelSize * sizeof(float));
        channelMats.push_back(channel);
    }
    
    cv::Mat result;
    cv::merge(channelMats, result);
    
    return result;
}

} // namespace SuperEigen 