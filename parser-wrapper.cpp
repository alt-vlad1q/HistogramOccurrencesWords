#include "parser-wrapper.hpp"

#include <core/worker-pool.hpp>
#include <core/facrory-task/factory-grammar-task.hpp>
#include <core/facrory-task/factory-pure-task.hpp>

#include <thread>

ParserWrapper::ParserWrapper(const std::string &filePath,
                             unsigned short countPage,
                             bool singleThread) :
    mSingleThread(singleThread),
    mCountPage(countPage),
    mFactoryTask(std::make_unique<core::factory::FactoryPureTask>())
{
    start(filePath);
}

ParserWrapper::~ParserWrapper()
{
    stop();
}

void ParserWrapper::start(const std::string &filePath)
{
    mInputSource.open(filePath, file_type::readonly),
    assert(mInputSource.is_open());
    mFileSize = mInputSource.size();

    mPool = std::make_unique<core::WorkerPool>(mSingleThread);
    mAccumulator = std::make_unique<core::Accumulator>();


    auto accumulator = [this](const core::Accumulator::queue_item_type & item) {
        mAccumulator->submitData(std::move(item));
    };

    auto transmitter = [this, &accumulator] (const std::string & chunk) {
        mCountBlock++;
        mPool->submitTask(mFactoryTask->generateTask(std::move(chunk), accumulator));
    };

    mFileSeparator = std::make_unique<core::FileSeparator>(mInputSource,
                                                           mPool->getCountWorkers(),
                                                           mFactoryTask->separator(),
                                                           transmitter,
                                                           mCountPage);
    try {
        mAccumulator->start(mFileSeparator->start());
    } catch (const std::length_error & e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

void ParserWrapper::stop()
{
    mCountBlock = 0;
    mFileSeparator.reset(nullptr);
    mAccumulator.reset(nullptr);
    mPool.reset(nullptr);

    if (mInputSource.is_open()) {
        mInputSource.close();
    }
}

const std::set<core::Accumulator::key_type,
               core::Accumulator::comp_type> ParserWrapper::getSubset(unsigned short count)
{
    return mAccumulator->getSubset(count);
}

