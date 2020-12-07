#include "accumulator.hpp"

namespace core {

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
    if (mWorker.joinable()) {
        std::cout << "Reader thread [" << mWorker.get_id();
        mWorker.join();
        std::cout << "] was joined" << std::endl;
    }
}

const std::set<Accumulator::key_type,
               Accumulator::comp_type> Accumulator::getSubset(unsigned short count) const
{
    const auto & byCount(container.get<multi_index::Element::ByCount>());
    auto begin(byCount.rbegin());
    const auto end(byCount.rend());

    auto comp = [](cr_key_type a, cr_key_type b) {
        return (a.first < b.first);
    };

    std::set<Accumulator::key_type,
                   Accumulator::comp_type> result(comp);
    // FIXME: check last element
    for (int i = 0; begin != end && i < 15; ++i, ++begin)
    {
        result.emplace(begin->count, begin->word);
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

    while(mAlive && countBlock--) {
        mResultsQueue.wait_and_pop(items);

        for (const auto & [word, count]: items) {
            auto & byWordContainer(container.get<multi_index::Element::ByWord>());
            auto emplaceResult(byWordContainer.emplace(word));
            byWordContainer.modify(emplaceResult.first, [c = count](multi_index::Element & element){
                element.count += c;
            });
        }
        std::cout << "finished " << countBlock << " [" << mCountBlock << "]" << std::endl;
    }
    std::cout << "finished" << std::endl;
}

} // core
