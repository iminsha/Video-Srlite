#ifndef SuperResolution_H
#define SuperResolution_H

#include <string>
#include <memory>
#include <opencv2/opencv.hpp>
#include "../SuperEigen/include/SuperResEngine.h"

class SuperResolutionProcessor {
public:
    SuperResolutionProcessor(const std::string& model_path);

    // 主函数：输入一帧 BGR 图，返回超分后 BGR 图
    cv::Mat Process(const cv::Mat& input_bgr);

private:
    std::string model_path_;
    std::unique_ptr<SuperEigen::SuperResEngine> engine_;
};

#endif // SuperResolution_H
