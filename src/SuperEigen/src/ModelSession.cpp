#include "../include/ModelSession.h"
#include "../../Utils/Logger.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace SuperEigen {

ModelSession::ModelSession(const SuperResConfig& config)
    : config_(config)
    , initialized_(false)
    , env_(ORT_LOGGING_LEVEL_WARNING, "SuperResModelSession")
    , memoryInfo_(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)) {
    configureSession();
}

ModelSession::~ModelSession() {
    // 清理资源
}

bool ModelSession::initialize(const std::string& modelPath) {
    try {
        // 加载模型
        #ifdef _WIN32
            std::wstring widestr = std::wstring(modelPath.begin(), modelPath.end());
            session_ = std::make_unique<Ort::Session>(env_, widestr.c_str(), sessionOptions_);
        #else
            session_ = std::make_unique<Ort::Session>(env_, modelPath.c_str(), sessionOptions_);
        #endif
        
        // 提取模型元数据
        extractModelMetadata();
        
        // 预热
        if (config_.enableOptimization) {
            warmUp();
        }
        
        initialized_ = true;
        LOG_INFO("Model loaded successfully from: " + modelPath);
        LOG_INFO("Using provider: " + getProviderName());
        
        return true;
    } catch (const Ort::Exception& e) {
        LOG_ERROR("Failed to initialize model: " + std::string(e.what()));
        initialized_ = false;
        return false;
    }
}

Ort::Value ModelSession::inference(const Ort::Value& inputTensor) {
    if (!initialized_) {
        throw std::runtime_error("Model session not initialized");
    }
    
    std::lock_guard<std::mutex> lock(sessionMutex_);
    
    try {
        // 准备输入
        std::vector<Ort::Value> inputTensors;
        inputTensors.push_back(std::move(const_cast<Ort::Value&>(inputTensor)));
        
        // 执行推理
        auto outputTensors = session_->Run(
            Ort::RunOptions{nullptr},
            inputNodeNames_.data(),
            inputTensors.data(),
            inputTensors.size(),
            outputNodeNames_.data(),
            outputNodeNames_.size()
        );
        
        return std::move(outputTensors[0]);
    } catch (const Ort::Exception& e) {
        throw std::runtime_error("Inference failed: " + std::string(e.what()));
    }
}



ModelSession::ModelInfo ModelSession::getModelInfo() const {
    ModelInfo info;
    info.inputNames = inputNames_;
    info.outputNames = outputNames_;
    info.inputShapes = inputShapes_;
    info.outputShapes = outputShapes_;
    info.scaleFactor = config_.scaleFactor;
    
    return info;
}

bool ModelSession::validateInput(const Ort::Value& tensor) const {
    if (!initialized_) {
        return false;
    }
    
    try {
        auto tensorInfo = tensor.GetTensorTypeAndShapeInfo();
        auto shape = tensorInfo.GetShape();
        
        // 检查维度数量
        if (shape.size() != inputShapes_[0].size()) {
            return false;
        }
        
        // 检查通道数（假设NCHW格式）
        if (shape.size() >= 2 && shape[1] != inputShapes_[0][1]) {
            return false;
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

bool ModelSession::setDynamicInputShape(const std::vector<int64_t>& shape) {
    // 某些模型支持动态输入形状
    // 这里可以根据需要实现
    return false;
}

void ModelSession::configureSession() {
    sessionOptions_.SetIntraOpNumThreads(config_.numThreads);
    
    if (config_.enableOptimization) {
        sessionOptions_.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
    }
    
    if (config_.enableMemoryPattern) {
        sessionOptions_.EnableMemPattern();
    }
    
    // 配置设备
    if (config_.isGPU()) {
        configureGPU();
    } else {
        configureCPU();
    }
}

void ModelSession::configureCPU() {
    // CPU配置已经在基本设置中完成
    LOG_DEBUG("Configured for CPU execution with " + std::to_string(config_.numThreads) + " threads");
}

void ModelSession::configureGPU() {
#ifdef USE_CUDA
    try {
        OrtCUDAProviderOptions cuda_options;
        cuda_options.device_id = config_.deviceId;
        cuda_options.cudnn_conv_algo_search = OrtCudnnConvAlgoSearchExhaustive;
        cuda_options.gpu_mem_limit = SIZE_MAX;
        
        sessionOptions_.AppendExecutionProvider_CUDA(cuda_options);
        LOG_DEBUG("Configured for GPU execution on device " + std::to_string(config_.deviceId));
    } catch (const Ort::Exception& e) {
        LOG_WARNING("Failed to configure GPU, falling back to CPU: " + std::string(e.what()));
        // 回退到CPU
    }
#else
    LOG_WARNING("CUDA support not compiled, using CPU");
#endif
}

void ModelSession::extractModelMetadata() {
    Ort::AllocatorWithDefaultOptions allocator;
    
    // 获取输入信息
    size_t numInputNodes = session_->GetInputCount();
    inputNames_.clear();
    inputShapes_.clear();
    inputNodeNames_.clear();
    
    for (size_t i = 0; i < numInputNodes; i++) {
        // 获取输入名称
        auto inputName = session_->GetInputNameAllocated(i, allocator);
        inputNames_.push_back(inputName.get());
        inputNodeNames_.push_back(inputNames_.back().c_str());
        
        // 获取输入形状
        Ort::TypeInfo typeInfo = session_->GetInputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        inputShapes_.push_back(tensorInfo.GetShape());
    }
    
    // 获取输出信息
    size_t numOutputNodes = session_->GetOutputCount();
    outputNames_.clear();
    outputShapes_.clear();
    outputNodeNames_.clear();
    
    for (size_t i = 0; i < numOutputNodes; i++) {
        // 获取输出名称
        auto outputName = session_->GetOutputNameAllocated(i, allocator);
        outputNames_.push_back(outputName.get());
        outputNodeNames_.push_back(outputNames_.back().c_str());
        
        // 获取输出形状
        Ort::TypeInfo typeInfo = session_->GetOutputTypeInfo(i);
        auto tensorInfo = typeInfo.GetTensorTypeAndShapeInfo();
        outputShapes_.push_back(tensorInfo.GetShape());
    }
    
    // 打印模型信息
    std::string inputShapeStr = "Model Input: " + inputNames_[0] + " Shape: [";
    for (size_t i = 0; i < inputShapes_[0].size(); i++) {
        inputShapeStr += std::to_string(inputShapes_[0][i]);
        if (i < inputShapes_[0].size() - 1) inputShapeStr += ", ";
    }
    inputShapeStr += "]";
    LOG_DEBUG(inputShapeStr);
    
    std::string outputShapeStr = "Model Output: " + outputNames_[0] + " Shape: [";
    for (size_t i = 0; i < outputShapes_[0].size(); i++) {
        outputShapeStr += std::to_string(outputShapes_[0][i]);
        if (i < outputShapes_[0].size() - 1) outputShapeStr += ", ";
    }
    outputShapeStr += "]";
    LOG_DEBUG(outputShapeStr);
}

std::string ModelSession::getProviderName() const {
    if (!session_) {
        return "None";
    }
    
    // ONNX Runtime 1.15.1 doesn't have GetProviders method for Session
    // Return provider based on configuration
    if (config_.isGPU()) {
        return "CUDAExecutionProvider";
    } else {
        return "CPUExecutionProvider";
    }
}

void ModelSession::warmUp() {
    if (!initialized_) {
        return;
    }
    
    try {
        // 创建一个小的测试输入进行预热
        std::vector<int64_t> testShape = {1, 3, 64, 64}; // 小尺寸测试
        std::vector<float> testData(1 * 3 * 64 * 64, 0.5f);
        
        auto testInput = Ort::Value::CreateTensor<float>(
            memoryInfo_,
            testData.data(),
            testData.size(),
            testShape.data(),
            testShape.size()
        );
        
        // 执行几次推理进行预热
        for (int i = 0; i < 3; ++i) {
            std::vector<Ort::Value> inputs;
            inputs.push_back(std::move(Ort::Value::CreateTensor<float>(
                memoryInfo_,
                testData.data(),
                testData.size(),
                testShape.data(),
                testShape.size()
            )));
            
            auto outputs = session_->Run(
                Ort::RunOptions{nullptr},
                inputNodeNames_.data(),
                inputs.data(),
                inputs.size(),
                outputNodeNames_.data(),
                outputNodeNames_.size()
            );
        }
        
        LOG_DEBUG("Model warmup completed");
    } catch (const Ort::Exception& e) {
        LOG_WARNING("Warmup failed (non-critical): " + std::string(e.what()));
    }
}

} // namespace SuperEigen 