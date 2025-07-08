#pragma once
#include <opencv2/opencv.hpp>
#include <string>

struct FrameData {
    cv::Mat image; // 图像数据

    int width = 0; // 图像宽度
    int height = 0; // 图像高度
    int64_t pts = 0; // 时间戳
    double timestamp = 0.0; // 时间戳（秒）

    int frameIndex = -1; // 帧索引
    std::string sourceTag; // 源标签

    // 可选元信息
    std::string pixFormat = "bgr24"; // 像素格式
    std::string colorSpace = "bt709"; // 颜色空间
    bool fullRange = false; // 是否为全范围
    int bitDepth = 8; // 位深度
};
