#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>

// 各模块头文件
#include "Decoder/include/VideoDecoder.h"
#include "Decoder/include/AudioDecoder.h"
#include "SuperEigen/include/SuperResEngine.h"
#include "SyncVA/AVSyncManager.h"
#include "Encoder/Encoder.h"
#include "Utils/Logger.h"

/**
 * @brief 视频处理流水线测试
 * 
 * 完整流程：解码 -> 超分 -> 同步 -> 编码
 */
class VideoPipeline {
public:
    VideoPipeline() : frameCount_(0), processedFrames_(0) {}
    
    bool initialize(const std::string& inputPath, const std::string& outputPath) {
        inputPath_ = inputPath;
        outputPath_ = outputPath;
        
        LOG_INFO("Initializing video pipeline...");
        LOG_INFO("Input: " + inputPath_);
        LOG_INFO("Output: " + outputPath_);
        
        // 初始化解码器
        if (!initializeDecoders()) {
            LOG_ERROR("Failed to initialize decoders");
            return false;
        }
        
        // 初始化超分引擎
        if (!initializeSuperRes()) {
            LOG_ERROR("Failed to initialize super resolution engine");
            return false;
        }
        
        // 初始化同步管理器
        syncManager_ = std::make_unique<AVSyncManager>();
        
        // 初始化编码器（延迟到获得第一帧尺寸后）
        encoderInitialized_ = false;
        
        LOG_INFO("Pipeline initialized successfully");
        return true;
    }
    
    bool process() {
        LOG_INFO("Starting video processing...");
        
        auto startTime = std::chrono::steady_clock::now();
        
        // 处理循环（处理完整视频）
        bool hasVideoFrames = true;
        bool hasAudioFrames = (audioDecoder_ != nullptr);
        int processedVideoFrames = 0;
        int max_frame = 20;
        while ((hasVideoFrames || hasAudioFrames) && processedVideoFrames < max_frame) {
            // 解码视频帧
            if (hasVideoFrames) {
                FrameData videoFrame;
                if (videoDecoder_->readNextFrame(videoFrame)) {
                    frameCount_++;
                    processedVideoFrames++;
                    
                    // 超分处理
                    if (!processSuperResolution(videoFrame)) {
                        LOG_ERROR("Failed to process super resolution for frame " + std::to_string(frameCount_));
                        break;
                    }
                    
                    // 推入同步管理器
                    syncManager_->pushVideo(videoFrame);
                    
                    LOG_INFO("Processed video frame " + std::to_string(frameCount_) + 
                              " @ " + std::to_string(videoFrame.timestamp) + "s");
                } else {
                    hasVideoFrames = false;
                    LOG_INFO("Video decoding completed, total frames: " + std::to_string(frameCount_));
                }
            }
            
            // 解码音频帧
            if (hasAudioFrames && audioDecoder_) {
                AudioFrameData audioFrame;
                if (audioDecoder_->readNextFrame(audioFrame)) {
                    // 推入同步管理器
                    syncManager_->pushAudio(audioFrame);
                    
                    LOG_DEBUG("Processed audio frame @ " + std::to_string(audioFrame.timestamp) + "s");
                } else {
                    hasAudioFrames = false;
                    LOG_INFO("Audio decoding completed");
                }
            }
            
            // 处理同步输出
            while (syncManager_->hasNext()) {
                auto frame = syncManager_->popNext();
                
                if (!encoderInitialized_) {
                    if (!initializeEncoder(frame)) {
                        LOG_ERROR("Failed to initialize encoder");
                        return false;
                    }
                    encoderInitialized_ = true;
                }
                
                // 调试：保存前几帧图像
                if (processedFrames_ < 3) {
                    if (AVSyncManager::isVideoFrame(frame)) {
                        const auto& videoFrame = AVSyncManager::getVideoFrame(frame);
                        std::string debugPath = "VideoSR-Lite/resource/image" + std::to_string(processedFrames_) + ".png";
                        cv::imwrite(debugPath, videoFrame.image);
                        LOG_INFO("Saved debug frame: " + debugPath + " size: " + 
                                std::to_string(videoFrame.image.cols) + "x" + std::to_string(videoFrame.image.rows));
                    }
                }
                
                if (!encoder_->push(frame)) {
                    LOG_ERROR("Failed to encode frame");
                    return false;
                }
                
                processedFrames_++;
                
                // 每100帧输出一次进度
                if (processedFrames_ % 100 == 0) {
                    LOG_INFO("Encoded " + std::to_string(processedFrames_) + " frames");
                }
            }
        }
        
        // 处理剩余的同步帧
        while (syncManager_->hasNext()) {
            auto frame = syncManager_->popNext();
            if (!encoder_->push(frame)) {
                LOG_ERROR("Failed to encode remaining frame");
                return false;
            }
            processedFrames_++;
        }
        
        // 刷新编码器
        if (!encoder_->flush()) {
            LOG_ERROR("Failed to flush encoder");
            return false;
        }
        
        // 关闭编码器
        if (!encoder_->close()) {
            LOG_ERROR("Failed to close encoder");
            return false;
        }
        
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        
        LOG_INFO("Processing completed successfully!");
        LOG_INFO("Total processing time: " + std::to_string(duration.count()) + " seconds");
        LOG_INFO("Total frames processed: " + std::to_string(processedFrames_));
        LOG_INFO("Output file: " + outputPath_);
        
        return true;
    }

private:
    // 组件
    std::unique_ptr<VideoDecoder> videoDecoder_;
    std::unique_ptr<AudioDecoder> audioDecoder_;
    std::unique_ptr<SuperEigen::SuperResEngine> superResEngine_;
    std::unique_ptr<AVSyncManager> syncManager_;
    std::unique_ptr<Encoder> encoder_;
    
    // 状态
    std::string inputPath_;
    std::string outputPath_;
    int frameCount_;
    int processedFrames_;
    bool encoderInitialized_;
    
    bool initializeDecoders() {
        // 初始化视频解码器
        videoDecoder_ = std::make_unique<VideoDecoder>();
        if (!videoDecoder_->open(inputPath_)) {
            LOG_ERROR("Failed to open video file: " + inputPath_);
            return false;
        }
        
        // 初始化音频解码器（可选）
        audioDecoder_ = std::make_unique<AudioDecoder>();
        bool hasAudio = audioDecoder_->open(inputPath_);
        if (!hasAudio) {
            LOG_WARNING("No audio stream found, processing video only");
            audioDecoder_.reset();  // 清空音频解码器
        }
        
        LOG_INFO("Decoders initialized successfully");
        return true;
    }
    
    bool initializeSuperRes() {
        superResEngine_ = std::make_unique<SuperEigen::SuperResEngine>();
        
        if (!superResEngine_->initialize(SuperEigen::SuperResEngine::getDefaultModelPath(), false, 0)) {
            LOG_ERROR("Failed to initialize super resolution engine");
            return false;
        }
        
        LOG_INFO("Super resolution engine initialized successfully");
        return true;
    }
    
    bool initializeEncoder(const AVSyncManager::FrameVariant& firstFrame) {
        encoder_ = std::make_unique<Encoder>();
        
        EncoderConfig config;
        config.outputPath = outputPath_;
        config.format = "mp4";
        
        // 从第一帧获取视频配置
        if (AVSyncManager::isVideoFrame(firstFrame)) {
            const auto& videoFrame = AVSyncManager::getVideoFrame(firstFrame);
            config.videoWidth = videoFrame.width;
            config.videoHeight = videoFrame.height;
            LOG_INFO("Detected video size: " + std::to_string(config.videoWidth) + "x" + std::to_string(config.videoHeight));
        }
        
        // 音频配置（如果有音频流）
        if (audioDecoder_) {
            config.audioSampleRate = 48000;
            config.audioChannels = 2;
            config.audioBitrate = 128000;
        } else {
            // 没有音频流，设置为0禁用音频编码
            config.audioSampleRate = 0;
            config.audioChannels = 0;
            config.audioBitrate = 0;
        }
        
        // 视频编码配置 - 高质量压缩
        config.videoBitrate = 0;        // 0表示使用CRF模式
        config.videoFrameRate = 30.0;
        config.videoPreset = "ultrafast";  // 快速编码
        config.videoCRF = 18;           // CRF=18为高质量压缩
        
        if (!encoder_->init(config)) {
            LOG_ERROR("Failed to initialize encoder");
            return false;
        }
        
        LOG_INFO("Encoder initialized successfully");
        return true;
    }
    
    bool processSuperResolution(FrameData& frame) {
        if (!superResEngine_) {
            LOG_ERROR("Super resolution engine not initialized");
            return false;
        }
        
        cv::Mat outputImage = superResEngine_->Process(frame.image);
        if (outputImage.empty()) {
            LOG_ERROR("Failed to process super resolution");
            return false;
        }
        
        // 更新帧数据
        frame.image = outputImage;
        frame.width = outputImage.cols;
        frame.height = outputImage.rows;
        
        return true;
    }
};

int main(int argc, char* argv[]) {
    
    // 配置日志系统
    Logger::getInstance().setLogLevel(Logger::LogLevel::INFO);
    Logger::getInstance().setLogToConsole(true);
    
    std::cout << "=== 视频处理流水线测试 ===" << std::endl;
    
    // 解析命令行参数
    std::string inputPath = "../resource/Vedio/vedio.mp4";  // 默认输入文件
    std::string outputPath = "../resource/output_enhanced.mp4";  // 输出文件
    
    if (argc >= 2) {
        inputPath = argv[1];
    }
    if (argc >= 3) {
        outputPath = argv[2];
    }
    
    std::cout << "输入文件: " << inputPath << std::endl;
    std::cout << "输出文件: " << outputPath << std::endl;
    
    // 创建并运行流水线
    VideoPipeline pipeline;
    
    if (!pipeline.initialize(inputPath, outputPath)) {
        std::cerr << "Failed to initialize pipeline" << std::endl;
        return -1;
    }
    
    if (!pipeline.process()) {
        std::cerr << "Failed to process video" << std::endl;
        return -1;
    }
    
    std::cout << "=== 处理完成 ===" << std::endl;
    return 0;
} 