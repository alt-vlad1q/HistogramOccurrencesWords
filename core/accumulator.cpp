#include "accumulator.hpp"

namespace core {

Accumulator::Accumulator(visualize_type visualize, progress_type progress) :
    mVisualizer(visualize),
    mProgress(progress)
{}

Accumulator::~Accumulator()
{
    stop();
}

void Accumulator::submitData(const queue_item_type &data)
{
    mResultsQueue.emplace(std::move(data));
}

void Accumulator::start(size_t countBlock)
{
    mCountBlock = countBlock;
    mAlive = true;

    mWorker = std::thread(&Accumulator::taskForThread, this);
}

void Accumulator::stop()
{
    mAlive = false;

    if (mWorker.joinable()) {
        mWorker.join();
    }
}

Accumulator::extracts_map Accumulator::getSubset(ushort count) const
{
    const auto & byCount {container.get<multi_index::Element::ByCount>()};
    auto begin {byCount.rbegin()};
    const auto end {byCount.rend()};
    extracts_map result {};
    for (int i = 0; begin != end && i < count; ++i, ++begin) {
        result.emplace(begin->word, begin->count);
    }
    return result;
}


void Accumulator::taskForThread()
{
    queue_item_type items;
    auto countBlock = mCountBlock;

    while(mAlive && countBlock) {
        if (!mResultsQueue.try_pop(items))
            continue;

        for (const auto & [word, count]: items) {
            auto & byWordContainer(container.get<multi_index::Element::ByWord>());
            auto emplaceResult(byWordContainer.emplace(word));
            byWordContainer.modify(emplaceResult.first, [c = count](multi_index::Element & element){
                element.count += c;
            });
        }
        --countBlock;
        mVisualizer(getSubset());
        mProgress(static_cast<ushort>(100 - countBlock * 100 / mCountBlock));
    }
}

} // core
