#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <opencv2/opencv.hpp>

class PostProcessor {
public:
    PostProcessor();
    ~PostProcessor();
    
    cv::Mat sharpen(const cv::Mat& input, float strength);
    cv::Mat adjustContrast(const cv::Mat& input, float contrast);

private:
    // 简化实现，不需要成员变量
};

#endif // POSTPROCESSOR_H 