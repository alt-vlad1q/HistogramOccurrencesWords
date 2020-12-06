#include "accumulator.hpp"

namespace core {

Accumulator::Accumulator(size_t dataSize) :
    mDataSize(dataSize)
{
    std::cout << "data size = " << mDataSize << std::endl;
    run();
}

Accumulator::~Accumulator()
{
    stop();
}

void Accumulator::submitData(const Accumulator::queue_item_type &data, bool lastBlock)
{
    mResultsQueue.emplace(std::move(data));
    if (lastBlock)
        mAlive = false;
}

void Accumulator::run()
{
    mAlive = true;
    mWorker = std::thread(&Accumulator::taskForThread, this);
}

void Accumulator::stop()
{
    mAlive = true;
    if (mWorker.joinable())
        mWorker.join();
}

const std::set<Accumulator::key_type,
               Accumulator::comp_type> Accumulator::getSubset(unsigned short count)
{
    auto comp = [](cr_key_type a, cr_key_type b) {
        return (a.second < b.second);
    };

    std::unique_lock<std::mutex> lk(mut);
    const auto first = mResults.cbegin();
    const auto secont = std::next(first, count < mResults.size() ? count : mResults.size());

    return {first, secont, comp};
}

void Accumulator::taskForThread()
{
    while(mAlive) {
        queue_item_type items;

        if (!mResultsQueue.try_pop(items))
            continue;

        for (const auto &item : items) {
            bool countedItem {false};
            std::unique_lock<std::mutex> lk(mut);
            for (auto &result : mResults) {
                if (result.second == item.first) {
                    const auto storedCount = result.first;
                    mResults.erase(result);
                    mResults.emplace(storedCount + item.second, item.first);
                    countedItem = true;
                    break;
                }
            }

            if (countedItem)
                continue;

            mResults.emplace(item.second, item.first);
        }
    }
}

} // core
