#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <QElapsedTimer>
#include <QThreadPool>
#include <QRunnable>
#include <QAtomicInt>
#include <opencv2/opencv.hpp>
#include <memory>

// 前向声明
namespace SuperEigen {
class SuperResEngine;
}

// 处理任务结构
struct ProcessingTask {
    QString id;           // 任务ID
    cv::Mat inputImage;   // 输入图像
    QString filePath;     // 文件路径（可选）
    int scaleFactor;      // 缩放倍数
    
    ProcessingTask() : scaleFactor(2) {}
    ProcessingTask(const QString& taskId, const cv::Mat& image, int scale = 2)
        : id(taskId), inputImage(image.clone()), scaleFactor(scale) {}
};

class ImageProcessor : public QObject
{
    Q_OBJECT
    friend class ProcessingTaskRunnable;

public:
    explicit ImageProcessor(QObject *parent = nullptr);
    ~ImageProcessor();

    // 控制方法
    void start();
    void stop();
    void pause();
    void resume();
    
    // 任务管理
    void addTask(const ProcessingTask& task);
    void clearTasks();
    int pendingTaskCount() const;

    // 线程池配置
    void setMaxThreadCount(int maxThreads);
    int maxThreadCount() const;
    int activeThreadCount() const;

    // 引擎状态
    bool isEngineReady() const;
    void initializeEngine();

public slots:
    void processNextTask();

signals:
    // 处理结果信号
    void imageProcessed(const QString& taskId, const cv::Mat& originalImage, const cv::Mat& enhancedImage, qint64 processingTime);
    void processingError(const QString& taskId, const QString& error);
    void processingProgress(int current, int total);
    
    // 状态信号
    void engineInitialized(bool success);
    void processingStarted();
    void processingStopped();
    void processingPaused();
    void processingResumed();



private:
    void processTask(const ProcessingTask& task);
    bool initializeSuperResEngine();

private:
    // 线程池管理
    QThreadPool* m_threadPool;

    // 任务队列
    mutable QMutex m_taskMutex;
    QQueue<ProcessingTask> m_taskQueue;
    QWaitCondition m_taskCondition;

    // 控制状态
    bool m_isRunning;
    bool m_isPaused;
    bool m_shouldStop;
    QAtomicInt m_activeTaskCount;

    // 超分辨率引擎
    std::unique_ptr<SuperEigen::SuperResEngine> m_srEngine;
    bool m_engineReady;
    mutable QMutex m_engineMutex;

    // 统计
    int m_totalTasks;
    int m_processedTasks;
};

// 处理任务运行器
class ProcessingTaskRunnable : public QRunnable
{
public:
    ProcessingTaskRunnable(ImageProcessor* processor, const ProcessingTask& task);
    void run() override;

private:
    ImageProcessor* m_processor;
    ProcessingTask m_task;
};

#endif // IMAGEPROCESSOR_H 