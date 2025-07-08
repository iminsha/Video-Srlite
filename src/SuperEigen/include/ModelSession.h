#pragma once
#include <onnxruntime_cxx_api.h>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include "SuperResConfig.h"

namespace SuperEigen {

/**
 * @brief ONNX模型会话管理器
 * 负责模型加载、推理执行、设备选择（GPU/CPU）
 */
class ModelSession {
public:
    explicit ModelSession(const SuperResConfig& config);
    ~ModelSession();

    /**
     * @brief 初始化模型会话
     * @param modelPath ONNX模型文件路径
     * @return 是否初始化成功
     */
    bool initialize(const std::string& modelPath);

    /**
     * @brief 执行推理
     * @param inputTensor 输入张量
     * @return 输出张量
     */
    Ort::Value inference(const Ort::Value& inputTensor);



    /**
     * @brief 获取内存信息对象
     * @return ONNX内存信息
     */
    const Ort::MemoryInfo& getMemoryInfo() const { return memoryInfo_; }

    /**
     * @brief 获取模型信息
     */
    struct ModelInfo {
        std::vector<std::string> inputNames;
        std::vector<std::string> outputNames;
        std::vector<std::vector<int64_t>> inputShapes;
        std::vector<std::vector<int64_t>> outputShapes;
        int scaleFactor;
    };
    
    ModelInfo getModelInfo() const;

    /**
     * @brief 验证输入张量形状
     * @param tensor 输入张量
     * @return 是否有效
     */
    bool validateInput(const Ort::Value& tensor) const;

    /**
     * @brief 获取是否已初始化
     */
    bool isInitialized() const { return initialized_; }

    /**
     * @brief 设置动态输入形状（某些模型支持）
     */
    bool setDynamicInputShape(const std::vector<int64_t>& shape);

private:
    const SuperResConfig& config_;
    bool initialized_;
    
    // ONNX Runtime 组件
    Ort::Env env_;
    Ort::SessionOptions sessionOptions_;
    std::unique_ptr<Ort::Session> session_;
    Ort::MemoryInfo memoryInfo_;
    Ort::AllocatorWithDefaultOptions allocator_;

    // 模型元数据
    std::vector<std::string> inputNames_;
    std::vector<std::string> outputNames_;
    std::vector<std::vector<int64_t>> inputShapes_;
    std::vector<std::vector<int64_t>> outputShapes_;
    
    // 输入输出节点名称（C风格字符串）
    std::vector<const char*> inputNodeNames_;
    std::vector<const char*> outputNodeNames_;

    // 线程安全
    mutable std::mutex sessionMutex_;

    // 私有方法
    void configureSession();
    void configureCPU();
    void configureGPU();
    void extractModelMetadata();
    std::string getProviderName() const;
    
    // 缓存管理
    void warmUp();
};

} // namespace SuperEigen 