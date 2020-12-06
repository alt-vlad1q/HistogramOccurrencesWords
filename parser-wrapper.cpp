#include "parser-wrapper.hpp"

ParserWrapper::ParserWrapper(const std::string &filePath,
                             bool oneThread,
                             unsigned short countPage) :
    mInputSource(filePath, file_type::readonly),
    mCountBlock(0),
    mPool(),
    mFactoryTask(std::make_unique<core::factory::FactoryPureTask>())
{
    assert(mInputSource.is_open());
    mFileSize = mInputSource.size();
    mAccumulator = std::make_unique<core::Accumulator>(mFileSize);

    auto accumulator = [this](const core::Accumulator::queue_item_type & item) {
        mAccumulator->submitData(std::move(item), !mCountBlock ? true : false);
        mCountBlock--;
    };

    auto transmitter = [this, &accumulator, oneThread] (const std::string & chunk) {
        mCountBlock++;
        if (oneThread)
            mFactoryTask->generateTask(std::move(chunk), accumulator)();
        else
            mPool.submitTask(mFactoryTask->generateTask(std::move(chunk), accumulator));
    };

    try {
        FileSeparator fsep (mInputSource, mFactoryTask->separator(), transmitter, countPage);
    } catch (const std::length_error & e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

ParserWrapper::~ParserWrapper()
{
    if (mInputSource.is_open()) {
        mInputSource.close();
    }
}

const std::set<core::Accumulator::key_type,
               core::Accumulator::comp_type> ParserWrapper::getSubset(unsigned short count)
{
    return mAccumulator->getSubset(count);
}

