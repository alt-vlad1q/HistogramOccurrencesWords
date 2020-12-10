#pragma once

#include <queue>
#include <condition_variable>

//! \brief The ThreadSafeQueue class
//! \details Потокобезопасная шаблонная очердь
template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;

    void emplace(T data)
    {
        std::lock_guard<std::mutex> lk(mMutex);
        mData.emplace(std::move(data));
        mCondVar.notify_one();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mMutex);
        if(mData.empty())
            return false;
        value=std::move(mData.front());
        mData.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mMutex);
        return mData.empty();
    }

private:

    mutable std::mutex mMutex;
    std::queue<T> mData;
    std::condition_variable mCondVar;
};
