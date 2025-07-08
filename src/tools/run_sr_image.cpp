#include "../SuperEigen/include/SuperResEngine.h"
#include "../Utils/Logger.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // 自动查找模型路径
    std::string model_path = SuperEigen::SuperResEngine::getDefaultModelPath();
    std::string input_path;
    std::string output_path;

    if (argc >= 3) {
        input_path = argv[1];
        output_path = argv[2];
        std::cout << "Processing image:\n";
        std::cout << "  Input:  " << input_path << "\n";
        std::cout << "  Output: " << output_path << "\n";
    } else {
        std::cout << "Usage: " << argv[0] << " <input_image> <output_image>\n";
        std::cout << "Example: " << argv[0] << " input.jpg output.png\n";
        return -1;
    }

    if (!fs::exists(model_path)) {
        std::cerr << "Model file not found: " << model_path << std::endl;
        return -1;
    }
    if (!fs::exists(input_path)) {
        std::cerr << "Input image not found: " << input_path << std::endl;
        return -1;
    }

    try {
        // 直接使用SuperResEngine
        SuperEigen::SuperResEngine engine;
        if (!engine.initialize(model_path)) {
            std::cerr << "Failed to initialize SuperRes engine" << std::endl;
            return -1;
        }

        std::cout << "🔥 Warming up model..." << std::endl;
        
        // 模型预热：创建一个小的测试图像进行第一次推理
        cv::Mat warmup_image = cv::Mat::zeros(256, 256, CV_8UC3);
        // 填充一些随机数据
        cv::randu(warmup_image, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
        
        // 执行预热推理（忽略结果）
        cv::Mat warmup_result = engine.Process(warmup_image);
        std::cout << "✅ Model warmed up successfully" << std::endl;

        // 使用OpenCV读取图片
        cv::Mat input_image = cv::imread(input_path, cv::IMREAD_COLOR);
        if (input_image.empty()) {
            std::cerr << "Failed to read image: " << input_path << std::endl;
            return -1;
        }

        std::cout << "Original image size: " << input_image.cols << "x" << input_image.rows << std::endl;

        // 直接进行超分处理（现在应该正常工作）
        cv::Mat output_image = engine.Process(input_image);
        
        std::cout << "Super-resolved image size: " << output_image.cols << "x" << output_image.rows << std::endl;

        if (output_image.empty()) {
            std::cerr << "Super resolution processing failed - output is empty" << std::endl;
            return -1;
        }

        // 确保输出目录存在
        auto parent_path = fs::path(output_path).parent_path();
        if (!parent_path.empty()) {
            fs::create_directories(parent_path);
        }
        
        // 保存结果图片
        if (!cv::imwrite(output_path, output_image)) {
            std::cerr << "Failed to save image to: " << output_path << std::endl;
            return -1;
        }
        
        std::cout << "✅ Saved super-resolved image to: " << output_path << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
} 