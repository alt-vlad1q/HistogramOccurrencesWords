#pragma once

#include "provider.hpp"

#include <core/file-separator.hpp>
#include <core/accumulator.hpp>

namespace core {
    class WorkerPool;
namespace factory {
    class FactoryTaskBase;
}}

class ParserWrapper
{
public:
    using file_type = core::FileSeparator::file_type;
    using file_size_type = core::FileSeparator::size_type;

    ParserWrapper(const std::string &filePath,
                  unsigned short countPage,
                  bool singleThread = false);
    ~ParserWrapper();
    ParserWrapper(const ParserWrapper &) = delete;
    ParserWrapper(ParserWrapper &&) = delete;
    ParserWrapper& operator=(const ParserWrapper &other) = delete;
    ParserWrapper& operator=(ParserWrapper &&other) = delete;

    void start(const std::string &filePath);
    void stop();

    Provider &getProvider();

private:
    file_type mInputSource;
    file_size_type mFileSize;
    std::atomic_int32_t mCountBlock;
    bool mSingleThread;
    unsigned short mCountPage;

    Provider mProvider;
    std::unique_ptr<core::WorkerPool> mPool;
    std::unique_ptr<core::FileSeparator> mFileSeparator;
    std::unique_ptr<core::factory::FactoryTaskBase> mFactoryTask;
    std::unique_ptr<core::Accumulator> mAccumulator;
};
