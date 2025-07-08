#pragma once

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <functional>
#include <memory>

// 任务类型定义
using Task = std::function<void()>;

// 工作线程类
class WorkerThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkerThread(QObject* parent = nullptr);
    ~WorkerThread();

    void addTask(Task task);
    void stop();
    bool isIdle() const;

protected:
    void run() override;

private:
    QMutex m_mutex;
    QWaitCondition m_condition;
    QQueue<Task> m_tasks;
    bool m_running;
    bool m_idle;
};

// 工作池类
class WorkerPool : public QObject
{
    Q_OBJECT

public:
    explicit WorkerPool(int threadCount = 1, QObject* parent = nullptr);
    ~WorkerPool();

    // 提交任务
    void submitTask(Task task);

    // 停止所有工作线程
    void stop();

    // 等待所有任务完成
    void waitForDone(int msecs = -1);

    // 获取状态
    int threadCount() const { return m_threads.size(); }
    bool isIdle() const;

signals:
    void allTasksCompleted();

private:
    std::vector<std::unique_ptr<WorkerThread>> m_threads;
    int m_currentThread;
    mutable QMutex m_mutex;
};