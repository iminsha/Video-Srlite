#include "../SuperEigen/include/SuperResEngine.h"
#include "../Utils/Logger.h"
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // 初始化引擎
    SuperEigen::SuperResEngine engine;
    std::string model_path = SuperEigen::SuperResEngine::getDefaultModelPath();
    
    if (!engine.initialize(model_path, false, 0)) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return -1;
    }
    
    // 读取测试图片
    cv::Mat input = cv::imread("resource/image/ls.jpeg", cv::IMREAD_COLOR);
    if (input.empty()) {
        std::cerr << "Failed to read image" << std::endl;
        return -1;
    }
    
    std::cout << "Input image: " << input.cols << "x" << input.rows << std::endl;
    
    // 检查输入图片的像素值
    double minVal, maxVal;
    cv::minMaxLoc(input, &minVal, &maxVal);
    std::cout << "Input range: [" << minVal << ", " << maxVal << "]" << std::endl;
    std::cout << "Input mean: " << cv::mean(input)[0] << std::endl;
    
    // 直接调用Process方法（与视频处理相同）
    cv::Mat output = engine.Process(input);
    
    std::cout << "Output image: " << output.cols << "x" << output.rows << std::endl;
    
    if (output.empty()) {
        std::cerr << "Failed to process" << std::endl;
        return -1;
    }
    
    // 检查输出图片的像素值
    double outMinVal, outMaxVal;
    cv::minMaxLoc(output, &outMinVal, &outMaxVal);
    std::cout << "Output range: [" << outMinVal << ", " << outMaxVal << "]" << std::endl;
    std::cout << "Output mean: " << cv::mean(output)[0] << std::endl;
    
    // 保存结果
    cv::imwrite("direct_test_output.png", output);
    std::cout << "Saved to direct_test_output.png" << std::endl;
    
    return 0;
} 