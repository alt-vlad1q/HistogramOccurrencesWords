#pragma once

#include "thread-safe-queue.hpp"

#include <atomic>
#include <thread>
#include <functional>

namespace core {

//! \brief The WorkerPool class
//! \details Пул потоков, которые, выполняют задачи из очереди
class WorkerPool
{
public:
    using task_type = std::function<void()>;

    enum class UnderlyingThread : unsigned short {
        Main,
        Reader,
        count = (Reader - Main + 1)
    };

    WorkerPool(bool singleThread = false);
    ~WorkerPool();
    WorkerPool(const WorkerPool &) = delete;
    WorkerPool(WorkerPool &&) = delete;
    WorkerPool& operator=(const WorkerPool &other) = delete;
    WorkerPool& operator=(WorkerPool &&other) = delete;

    unsigned short getCountWorkers() const;
    void submitTask(task_type task);

private:
    void taskForWorker();

private:
    const unsigned short mCountThreads;
    std::atomic_bool mAlive;
    ThreadSafeQueue<task_type> mTaskQueue;
    std::vector<std::thread> mWorkers;
};

} // core
