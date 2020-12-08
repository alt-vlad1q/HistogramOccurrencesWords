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
    mProvider(),
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

    auto visualize = [this](core::Accumulator::extracts_map map) {
        mProvider.run(std::move(map));
    };

    auto progress = [this](ushort value) {
        mProvider.setCompletedPercent(value);
    };

    mAccumulator = std::make_unique<core::Accumulator>(visualize, progress);


    auto accumulator = [this](const core::Accumulator::queue_item_type & item) {
        mAccumulator->submitData(std::move(item));
    };

    auto transmitter = [this, &accumulator] (const std::string & chunk) {
        mCountBlock++;
        mPool->submitTask(mFactoryTask->generateTask(std::move(chunk), accumulator));
    };

    try {
        mFileSeparator = std::make_unique<core::FileSeparator>(mInputSource,
                                                               mFactoryTask->separator(),
                                                               transmitter,
                                                               mPool->getCountWorkers(),
                                                               mCountPage);
    } catch (const std::length_error & e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    mAccumulator->start(mFileSeparator->countBlocks());
    std::cout << "blocks"  << mFileSeparator->countBlocks() << std::endl;

    mFileSeparator->start();
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

Provider &ParserWrapper::getProvider() {
    return mProvider;
}

