#pragma once
#include <string>
#include <unordered_map>

namespace SuperEigen {

/**
 * @brief 超分辨率配置结构体
 * 用于统一传递模型路径、倍率、后端类型等用户参数
 */
struct SuperResConfig {
    // 模型相关配置
    std::string modelPath;           // ONNX模型文件路径
    int scaleFactor = 2;            // 超分倍率 (2 or 4)
    std::string modelName = "RealESRGAN"; // 模型名称标识

    // 设备相关配置
    enum DeviceType {
        CPU = 0,
        GPU = 1,
        AUTO = 2  // 自动选择
    };
    DeviceType device = CPU;        // 推理设备类型
    int deviceId = 0;               // GPU设备ID (仅GPU模式有效)

    // 性能相关配置
    int numThreads = 4;             // CPU线程数
    bool enableOptimization = true;  // 是否启用优化
    bool enableMemoryPattern = true; // 是否启用内存模式优化

    // 处理相关配置
    float inputMean = 0.0f;         // 输入归一化均值
    float inputStd = 1.0f;          // 输入归一化标准差
    bool fp16Mode = false;          // 是否使用FP16推理
    
    // 批处理配置
    int batchSize = 1;              // 批处理大小
    
    // 额外配置选项
    std::unordered_map<std::string, std::string> extraOptions;

    // 构造函数
    SuperResConfig() = default;
    
    SuperResConfig(const std::string& model, int scale = 2, DeviceType dev = CPU) 
        : modelPath(model), scaleFactor(scale), device(dev) {}

    // 便捷方法
    bool isGPU() const { return device == GPU || (device == AUTO && checkGPUAvailable()); }
    
public:
    static bool checkGPUAvailable();
};

} // namespace SuperEigen 