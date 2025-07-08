#pragma once
#include <memory>
#include <mutex>
#include <functional>
#include "../../DataStruct/FrameData.h"
#include "SuperResConfig.h"
#include "ModelSession.h"
#include "PrePostProcessor.h"

namespace SuperEigen {

/**
 * @brief 超分辨率引擎 - 统一外部接口
 * 
 * 简化的四层架构：
 * 1. SuperResEngine（外部接口层）- 本类
 * 2. ModelSession（推理执行层）  
 * 3. PrePostProcessor（数据处理层）
 * 4. SuperResConfig（配置层）
 */
class SuperResEngine {
public:
    SuperResEngine();
    ~SuperResEngine();

    // ========== 基础接口 ==========
    
    /**
     * @brief 使用默认配置初始化
     * @return 是否成功
     */
    bool initializeDefault();

    /**
     * @brief 自定义初始化
     * @param modelPath 模型路径（空则使用默认）
     * @param useGPU 是否使用GPU
     * @param gpuId GPU设备ID
     * @return 是否成功
     */
    bool initialize(const std::string& modelPath = "", bool useGPU = false, int gpuId = 0);

    /**
     * @brief 处理单帧图像 - 兼容现有接口
     * @param input_bgr 输入BGR图像
     * @return 处理后的BGR图像
     */
    cv::Mat Process(const cv::Mat& input_bgr);

    /**
     * @brief 处理FrameData - 统一接口
     * @param input 输入帧数据
     * @return 处理后的帧数据
     */
    FrameData processFrame(const FrameData& input);

    /**
     * @brief 批量处理
     * @param inputs 输入帧数据列表
     * @return 处理后的帧数据列表
     */
    std::vector<FrameData> processBatch(const std::vector<FrameData>& inputs);

    // ========== 状态查询 ==========
    
    /**
     * @brief 是否已初始化
     */
    bool isInitialized() const { return initialized_; }

    /**
     * @brief 获取超分倍率
     */
    int getScaleFactor() const { return config_.scaleFactor; }

    /**
     * @brief 获取处理统计
     */
    struct ProcessingStats {
        size_t totalFrames;
        double avgTimeMs;
        double lastTimeMs;
        std::string modelPath;
        int scaleFactor;
        bool useGPU;
    };
    ProcessingStats getStats() const;

    // ========== 动态配置 ==========
    
    /**
     * @brief 切换模型
     * @param modelPath 新模型路径
     * @return 是否成功
     */
    bool switchModel(const std::string& modelPath);

    /**
     * @brief 切换设备
     * @param useGPU 是否使用GPU
     * @param gpuId GPU ID
     * @return 是否成功
     */
    bool switchDevice(bool useGPU, int gpuId = 0);

    /**
     * @brief 设置进度回调（批处理时）
     * @param callback 回调函数(当前, 总数)
     */
    void setProgressCallback(std::function<void(int, int)> callback);

    // ========== 静态工具 ==========
    
    /**
     * @brief 获取默认模型路径
     */
    static std::string getDefaultModelPath();

    /**
     * @brief 检查GPU支持
     */
    static bool isGPUSupported();

    /**
     * @brief 列出可用模型
     */
    static std::vector<std::string> getAvailableModels();

private:
    SuperResConfig config_;
    bool initialized_;
    
    // 核心组件
    std::unique_ptr<ModelSession> session_;
    std::unique_ptr<PrePostProcessor> processor_;
    
    // 统计信息
    mutable std::mutex statsMutex_;
    size_t processedFrames_;
    double totalProcessTime_;
    double lastProcessTime_;
    
    // 回调
    std::function<void(int, int)> progressCallback_;
    
    // 私有方法
    void updateFrameMetadata(FrameData& frame);
    void collectStats(double timeMs);
    cv::Mat processImageInternal(const cv::Mat& image);
};

} // namespace SuperEigen 