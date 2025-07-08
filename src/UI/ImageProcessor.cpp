#include "ImageProcessor.h"
#include "../SuperEigen/include/SuperResEngine.h"
#include <QDebug>
#include <QApplication>

ImageProcessor::ImageProcessor(QObject *parent)
    : QObject(parent)
    , m_threadPool(QThreadPool::globalInstance())
    , m_isRunning(false)
    , m_isPaused(false)
    , m_shouldStop(false)
    , m_activeTaskCount(0)
    , m_srEngine(nullptr)
    , m_engineReady(false)
    , m_totalTasks(0)
    , m_processedTasks(0)
{
    // 设置线程池最大线程数为CPU核心数
    m_threadPool->setMaxThreadCount(QThread::idealThreadCount());

    // 在后台线程初始化引擎
    QThread::create([this]() {
        initializeSuperResEngine();
    })->start();
}

ImageProcessor::~ImageProcessor()
{
    stop();
}

void ImageProcessor::start()
{
    if (m_isRunning) {
        return;
    }

    qDebug() << "ImageProcessor: 启动处理器，线程池最大线程数:" << m_threadPool->maxThreadCount();

    m_isRunning = true;
    m_shouldStop = false;
    m_isPaused = false;

    emit processingStarted();

    // 开始处理队列中的任务
    processNextTask();
}

void ImageProcessor::stop()
{
    if (!m_isRunning) {
        return;
    }
    
    qDebug() << "ImageProcessor: 停止处理";

    m_shouldStop = true;
    m_isRunning = false;
    m_isPaused = false;

    // 等待所有活动任务完成
    m_threadPool->waitForDone(5000);

    emit processingStopped();
}

void ImageProcessor::pause()
{
    if (!m_isRunning || m_isPaused) {
        return;
    }
    
    qDebug() << "ImageProcessor: 暂停处理";
    
    {
        QMutexLocker locker(&m_taskMutex);
        m_isPaused = true;
    }
    
    emit processingPaused();
}

void ImageProcessor::resume()
{
    if (!m_isRunning || !m_isPaused) {
        return;
    }
    
    qDebug() << "ImageProcessor: 恢复处理";
    
    {
        QMutexLocker locker(&m_taskMutex);
        m_isPaused = false;
        m_taskCondition.wakeAll();
    }
    
    emit processingResumed();
}

void ImageProcessor::addTask(const ProcessingTask& task)
{
    {
        QMutexLocker locker(&m_taskMutex);
        m_taskQueue.enqueue(task);
        m_totalTasks++;
        m_taskCondition.wakeOne();
    }
    
    qDebug() << "ImageProcessor: 添加任务" << task.id << "，队列长度:" << m_taskQueue.size();
    
    // 如果还未启动，自动启动
    if (!m_isRunning) {
        start();
    }
}

void ImageProcessor::clearTasks()
{
    QMutexLocker locker(&m_taskMutex);
    m_taskQueue.clear();
    qDebug() << "ImageProcessor: 清空任务队列";
}

int ImageProcessor::pendingTaskCount() const
{
    QMutexLocker locker(&m_taskMutex);
    return m_taskQueue.size();
}

bool ImageProcessor::isEngineReady() const
{
    return m_engineReady;
}

void ImageProcessor::initializeEngine()
{
    QThread::create([this]() {
        initializeSuperResEngine();
    })->start();
}

void ImageProcessor::processNextTask()
{
    if (!m_isRunning || m_shouldStop || m_isPaused) {
        return;
    }

    ProcessingTask task;
    bool hasTask = false;

    {
        QMutexLocker locker(&m_taskMutex);
        if (!m_taskQueue.isEmpty()) {
            task = m_taskQueue.dequeue();
            hasTask = true;
        }
    }

    if (hasTask) {
        // 创建任务运行器并提交到线程池
        ProcessingTaskRunnable* runnable = new ProcessingTaskRunnable(this, task);
        m_threadPool->start(runnable);
        m_activeTaskCount.fetchAndAddOrdered(1);

        // 继续处理下一个任务
        QMetaObject::invokeMethod(this, "processNextTask", Qt::QueuedConnection);
    }
}

void ImageProcessor::setMaxThreadCount(int maxThreads)
{
    m_threadPool->setMaxThreadCount(maxThreads);
    qDebug() << "ImageProcessor: 设置最大线程数为" << maxThreads;
}

int ImageProcessor::maxThreadCount() const
{
    return m_threadPool->maxThreadCount();
}

int ImageProcessor::activeThreadCount() const
{
    return m_activeTaskCount.loadAcquire();
}

void ImageProcessor::processTask(const ProcessingTask& task)
{
    if (task.inputImage.empty()) {
        emit processingError(task.id, "输入图像为空");
        return;
    }
    
    if (!m_engineReady) {
        emit processingError(task.id, "超分辨率引擎未就绪");
        return;
    }
    
    qDebug() << "ImageProcessor: 开始处理任务" << task.id;
    
    QElapsedTimer timer;
    timer.start();
    
    try {
        // 执行超分辨率处理
        cv::Mat enhancedImage = m_srEngine->Process(task.inputImage);
        
        if (enhancedImage.empty()) {
            emit processingError(task.id, "超分辨率处理失败，输出图像为空");
            return;
        }
        
        qint64 processingTime = timer.elapsed();
        qDebug() << "ImageProcessor: 任务" << task.id << "处理完成，用时" << processingTime << "ms";
        
        // 发送结果
        emit imageProcessed(task.id, task.inputImage, enhancedImage, processingTime);
        
    } catch (const std::exception& e) {
        emit processingError(task.id, QString("处理异常：%1").arg(e.what()));
    } catch (...) {
        emit processingError(task.id, "未知处理异常");
    }
}

bool ImageProcessor::initializeSuperResEngine()
{
    qDebug() << "ImageProcessor: 初始化超分辨率引擎...";
    
    try {
        m_srEngine = std::make_unique<SuperEigen::SuperResEngine>();
        m_engineReady = m_srEngine->initializeDefault();
        
        if (m_engineReady) {
            qDebug() << "ImageProcessor: 超分辨率引擎初始化成功";
        } else {
            qDebug() << "ImageProcessor: 超分辨率引擎初始化失败";
        }
        
        // 发送信号到主线程
        QMetaObject::invokeMethod(this, [this]() {
            emit engineInitialized(m_engineReady);
        }, Qt::QueuedConnection);
        
        return m_engineReady;
        
    } catch (const std::exception& e) {
        qDebug() << "ImageProcessor: 引擎初始化异常:" << e.what();
        m_engineReady = false;
        
        QMetaObject::invokeMethod(this, [this, e]() {
            emit engineInitialized(false);
        }, Qt::QueuedConnection);
        
        return false;
    }
}

// ProcessingTaskRunnable 实现
ProcessingTaskRunnable::ProcessingTaskRunnable(ImageProcessor* processor, const ProcessingTask& task)
    : m_processor(processor), m_task(task)
{
    setAutoDelete(true); // 任务完成后自动删除
}

void ProcessingTaskRunnable::run()
{
    if (!m_processor || m_processor->m_shouldStop) {
        m_processor->m_activeTaskCount.fetchAndAddOrdered(-1);
        return;
    }

    try {
        // 处理任务
        m_processor->processTask(m_task);

        // 更新统计
        m_processor->m_processedTasks++;
        emit m_processor->processingProgress(m_processor->m_processedTasks, m_processor->m_totalTasks);

    } catch (const std::exception& e) {
        qWarning() << "ProcessingTaskRunnable: 处理任务时发生异常:" << e.what();
        emit m_processor->processingError(m_task.id, QString("处理异常: %1").arg(e.what()));
    }

    // 减少活动任务计数
    m_processor->m_activeTaskCount.fetchAndAddOrdered(-1);

    // 继续处理下一个任务
    if (m_processor->m_isRunning && !m_processor->m_shouldStop) {
        QMetaObject::invokeMethod(m_processor, "processNextTask", Qt::QueuedConnection);
    }
}

#include "ImageProcessor.moc" 