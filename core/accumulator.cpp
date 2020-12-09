#include "accumulator.hpp"

namespace core {

Accumulator::Accumulator(Accumulator::visualize_type visualize, progress_type progress) :
    mVisualizer(visualize),
    mProgress(progress)
{}

Accumulator::~Accumulator()
{
    stop();
    std::cout << "dest Accumulator" << std::endl;
}

void Accumulator::submitData(const Accumulator::queue_item_type &data)
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

    std::cout << "mAlive = " << mAlive << std::endl;
    std::cout << "queue size = " << mResultsQueue.size() <<  std::endl;

    if (mWorker.joinable()) {
        std::cout << "Reader thread [" << mWorker.get_id();
        mWorker.join();
        std::cout << "] was joined" << std::endl;
    }
}

Accumulator::extracts_map Accumulator::getSubset(unsigned short count) const
{
    const auto & byCount(container.get<multi_index::Element::ByCount>());
    auto begin(byCount.rbegin());
    const auto end(byCount.rend());

    extracts_map result {};
    // FIXME: check last element
    for (int i = 0; begin != end && i < count; ++i, ++begin)
    {
        result.emplace(begin->word, begin->count);
    }
    return result;

//    std::unique_lock<std::mutex> lk(mut);
//    const auto first = mResults.cbegin();
//    const auto secont = std::next(first, count < mResults.size() ? count : mResults.size());

//    return {first, secont, comp};
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
        mVisualizer(getSubset(15));
        mProgress(static_cast<ushort>(100 - countBlock * 100 / mCountBlock));
    }
//    for (const auto & word : getSubset(15)) {
//        std::cout << word.first << " " << word.second << std::endl;
//    }
}

} // core
