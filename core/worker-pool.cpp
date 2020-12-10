#include "worker-pool.hpp"

#include <cassert>

namespace core {

WorkerPool::WorkerPool(bool singleThread) :
    mCountThreads(singleThread ? 1 : (std::thread::hardware_concurrency() -
                  static_cast<unsigned int>(WorkerPool::UnderlyingThread::count))),
    mAlive(true)
{
    assert (mCountThreads);

    try {
        for (unsigned number = 0; number < mCountThreads; ++number) {
            mWorkers.emplace_back(std::thread(&WorkerPool::taskForWorker, this));
        }
    } catch (...) {
        mAlive = false;
        throw;
    }
}

WorkerPool::~WorkerPool() {
    mAlive = false;
    for (auto & worker : mWorkers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

unsigned short WorkerPool::getCountWorkers() const
{
    return mCountThreads;
}

void WorkerPool::submitTask(WorkerPool::task_type task)
{
    mTaskQueue.emplace(std::move(task));
}

void WorkerPool::taskForWorker() {
    while(mAlive) {
        task_type task;
        if (mTaskQueue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
}

} // core
