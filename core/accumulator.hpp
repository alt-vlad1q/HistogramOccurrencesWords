#pragma once

#include "thread-safe-queue.hpp"

#include <atomic>
#include <thread>
#include <unordered_map>
#include <map>
#include <set>
#include <condition_variable>
#include <functional>

#include "multi-index-container.hpp"

namespace core {

class Accumulator
{
public:
    using queue_item_type = std::unordered_map<std::string, size_t>;
    using key_type = std::pair<size_t, std::string>;
    using cr_key_type = const key_type &;
    using result_type = std::set<key_type, std::greater<key_type>>;
    using comp_type = std::function<bool(cr_key_type, cr_key_type)>;

    Accumulator() = default;
    ~Accumulator();

    void start (size_t countBlock);
    void stop ();
    void submitData(const queue_item_type &data);
    const std::set<key_type, comp_type> getSubset(unsigned short count) const;

    multi_index::Container container;

private:
    void taskForThread ();

private:
    std::atomic_bool mAlive;
    size_t mCountBlock;

    std::thread mWorker;
    ThreadSafeQueue<queue_item_type> mResultsQueue;
    result_type mResults;
    mutable std::mutex mut;
};

} // core
