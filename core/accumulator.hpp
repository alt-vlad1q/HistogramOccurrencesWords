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

    using extracts_map = std::map<std::string, size_t>;

    using visualize_type = std::function<void(extracts_map)>;
    using progress_type = std::function<void(ushort)>;

    Accumulator(visualize_type visualize, progress_type progress);
    ~Accumulator();

    void start (size_t countBlock);
    void stop ();
    void submitData(const queue_item_type &data);
    extracts_map getSubset(unsigned short count) const;


private:
    void taskForThread ();

private:
    multi_index::Container container;
    std::atomic_bool mAlive;
    size_t mCountBlock;
    visualize_type mVisualizer;
    progress_type mProgress;

    std::thread mWorker;
    ThreadSafeQueue<queue_item_type> mResultsQueue;
    result_type mResults;
    mutable std::mutex mut;
};

} // core
