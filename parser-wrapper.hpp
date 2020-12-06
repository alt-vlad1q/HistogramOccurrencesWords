#pragma once

#include <core/file-separator.hpp>
#include <core/worker-pool.hpp>
#include <core/accumulator.hpp>
#include <core/facrory-task/factory-grammar-task.hpp>
#include <core/facrory-task/factory-pure-task.hpp>

class ParserWrapper
{
public:
    using file_type = FileSeparator::file_type;
    using file_size_type = FileSeparator::size_type;

    ParserWrapper(const std::string &filePath,
                  bool oneThread = false,
                  unsigned short countPage = 1);
    ~ParserWrapper();

    const std::set<core::Accumulator::key_type,
                   core::Accumulator::comp_type> getSubset(unsigned short count);
private:
    file_type mInputSource;
    file_size_type mFileSize;
    std::atomic_int mCountBlock;

    core::WorkerPool mPool;
    std::unique_ptr<core::factory::FactoryTaskBase> mFactoryTask;
    std::unique_ptr<core::Accumulator> mAccumulator;
};
