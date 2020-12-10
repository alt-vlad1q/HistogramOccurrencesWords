#include "parser-wrapper.hpp"

#include <core/worker-pool.hpp>
#include <core/facrory-task/factory-grammar-task.hpp>
#include <core/facrory-task/factory-pure-task.hpp>

#include <thread>
#include <iostream>

ParserWrapper::ParserWrapper(unsigned short countPage, bool singleThread) :
    mSingleThread(singleThread),
    mCountPage(countPage),
    mProvider([this](const std::string &filePath){start(filePath);}, [this]{stop();}),
    mFactoryTask(std::make_unique<core::factory::FactoryPureTask>())
{}

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
    mAccumulator = std::make_unique<core::Accumulator> (
                [this](auto map) {mProvider.deliverMap(std::move(map));},
                [this](auto value) {mProvider.progress(value);}
    );


    auto accumulator = [this](const core::Accumulator::queue_item_type & item) {
        mAccumulator->submitData(std::move(item));
    };

    auto transmitter = [this, &accumulator] (const std::string & chunk) {
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
    mFileSeparator->start();
}

void ParserWrapper::stop()
{
    mPool.reset(nullptr);
    mFileSeparator.reset(nullptr);
    mAccumulator.reset(nullptr);

    if (mInputSource.is_open()) {
        mInputSource.close();
    }
}

Provider &ParserWrapper::getProvider()
{
    return mProvider;
}

