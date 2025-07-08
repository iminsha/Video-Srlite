#include "WorkerPool.h"
#include <QDebug>

// WorkerThread 实现
WorkerThread::WorkerThread(QObject* parent)
    : QThread(parent), m_running(false), m_idle(true)
{
}

WorkerThread::~WorkerThread()
{
    stop();
    wait();
}

void WorkerThread::addTask(Task task)
{
    QMutexLocker locker(&m_mutex);
    m_tasks.enqueue(task);
    m_idle = false;
    m_condition.wakeOne();
}

void WorkerThread::stop()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;
    m_condition.wakeAll();
}

bool WorkerThread::isIdle() const
{
    QMutexLocker locker(const_cast<QMutex*>(&m_mutex));
    return m_idle && m_tasks.isEmpty();
}

void WorkerThread::run()
{
    m_running = true;
    qDebug() << "WorkerThread: 线程启动" << QThread::currentThreadId();

    while (m_running) {
        Task task;

        {
            QMutexLocker locker(&m_mutex);

            while (m_tasks.isEmpty() && m_running) {
                m_idle = true;
                m_condition.wait(&m_mutex);
            }

            if (!m_running) {
                break;
            }

            if (!m_tasks.isEmpty()) {
                task = m_tasks.dequeue();
                m_idle = false;
            }
        }

        if (task) {
            try {
                task();
            } catch (const std::exception& e) {
                qWarning() << "WorkerThread: 任务执行异常:" << e.what();
            }
        }
    }

    qDebug() << "WorkerThread: 线程结束" << QThread::currentThreadId();
}

// WorkerPool 实现
WorkerPool::WorkerPool(int threadCount, QObject* parent)
    : QObject(parent), m_currentThread(0)
{
    qDebug() << "WorkerPool: 创建工作池，线程数:" << threadCount;

    for (int i = 0; i < threadCount; ++i) {
        auto thread = std::make_unique<WorkerThread>(this);
        thread->start();
        m_threads.push_back(std::move(thread));
    }
}

WorkerPool::~WorkerPool()
{
    stop();
}

void WorkerPool::submitTask(Task task)
{
    if (m_threads.empty()) {
        qWarning() << "WorkerPool: 没有可用的工作线程";
        return;
    }

    QMutexLocker locker(&m_mutex);

    // 简单的轮询分配策略
    m_threads[m_currentThread]->addTask(task);
    m_currentThread = (m_currentThread + 1) % m_threads.size();
}

void WorkerPool::stop()
{
    qDebug() << "WorkerPool: 停止所有工作线程";

    for (auto& thread : m_threads) {
        thread->stop();
    }

    for (auto& thread : m_threads) {
        thread->wait();
    }
}

void WorkerPool::waitForDone(int msecs)
{
    Q_UNUSED(msecs)

    // 等待所有线程空闲
    bool allIdle = false;
    while (!allIdle) {
        allIdle = true;
        for (const auto& thread : m_threads) {
            if (!thread->isIdle()) {
                allIdle = false;
                break;
            }
        }

        if (!allIdle) {
            QThread::msleep(10);
        }
    }

    emit allTasksCompleted();
}

bool WorkerPool::isIdle() const
{
    for (const auto& thread : m_threads) {
        if (!thread->isIdle()) {
            return false;
        }
    }
    return true;
}

#include "WorkerPool.moc"