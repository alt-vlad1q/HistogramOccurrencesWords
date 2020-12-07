#pragma once

#include "thread-safe-queue.hpp"

#include <atomic>
#include <thread>
#include <functional>
#include <iostream>

namespace core {

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

    unsigned short getCountWorkers();
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
