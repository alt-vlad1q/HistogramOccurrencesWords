#pragma once

#include "thread-safe-queue.hpp"
#include "multi-index-container.hpp"

#include <atomic>
#include <thread>

namespace core {

//! \brief The Accumulator class
//! \details Содержит в себе очередь, обработанных блоков файла.
//! Производит слияние блоков в один и отправляет выборку в gui
class Accumulator
{
    enum class Settings {
        TopCount = 15,
    };
public:
    using queue_item_type = std::unordered_map<std::string, size_t>;
    using key_type = std::pair<size_t, std::string>;
    using cr_key_type = const key_type &;
    using result_type = std::set<key_type, std::greater<key_type>>;
    using extracts_map = std::map<std::string, size_t>;
    using visualize_type = std::function<void(extracts_map)>;
    using progress_type = std::function<void(ushort)>;

    Accumulator(visualize_type visualize, progress_type progress);
    ~Accumulator();
    Accumulator(const Accumulator &) = delete;
    Accumulator(Accumulator &&) = delete;
    Accumulator& operator=(const Accumulator &other) = delete;
    Accumulator& operator=(Accumulator &&other) = delete;

    void start (size_t countBlock);
    void stop ();
    void submitData(const queue_item_type &data);
    extracts_map getSubset(ushort count = static_cast<ushort>(Settings::TopCount)) const;

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
};

} // core
