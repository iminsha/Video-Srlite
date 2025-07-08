#pragma once
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>
#include <vector>
#include <memory>
#include "SuperResConfig.h"

namespace SuperEigen {

/**
 * @brief 预处理和后处理器
 * 负责图像数据与张量之间的转换，包括色彩空间转换、归一化等
 */
class PrePostProcessor {
public:
    explicit PrePostProcessor(const SuperResConfig& config);
    ~PrePostProcessor() = default;

    /**
     * @brief 预处理：将cv::Mat转换为ONNX张量
     * @param input BGR格式的输入图像
     * @param memoryInfo ONNX内存信息对象
     * @return ONNX张量
     */
    Ort::Value preprocess(const cv::Mat& input, const Ort::MemoryInfo& memoryInfo);

    /**
     * @brief 后处理：将ONNX张量转换回cv::Mat
     * @param outputTensor 模型输出张量
     * @param originalSize 原始图像尺寸(用于验证)
     * @return BGR格式的输出图像
     */
    cv::Mat postprocess(const Ort::Value& outputTensor, const cv::Size& originalSize);



    // 获取预期的输入/输出形状
    std::vector<int64_t> getInputShape(const cv::Mat& image) const;
    std::vector<int64_t> getOutputShape(const cv::Mat& image) const;

private:
    const SuperResConfig& config_;
    
    // 内部辅助方法
    cv::Mat bgrToRgb(const cv::Mat& bgr);
    cv::Mat rgbToBgr(const cv::Mat& rgb);
    std::vector<float> normalize(const cv::Mat& image);
    cv::Mat denormalize(const std::vector<float>& tensor, int width, int height);
    
    // 数据布局转换 (HWC <-> CHW)
    std::vector<float> hwcToChw(const cv::Mat& image);
    cv::Mat chwToHwc(const std::vector<float>& tensor, int channels, int height, int width);
    
    // 缓冲区管理
    mutable std::vector<float> preprocessBuffer_;
    mutable std::vector<float> postprocessBuffer_;
};

} // namespace SuperEigen 