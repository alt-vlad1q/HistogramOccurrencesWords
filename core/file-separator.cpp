#include "file-separator.hpp"

#include <cmath>
#include <exception>
#include <iostream>
#include <thread>

namespace core {

FileSeparator::FileSeparator(file_type &file,
                             unsigned short countWorkers,
                             const std::string &separator,
                             submit_type submit,
                             ushort countPage) :
    mAlive(true),
    mInputSource(file),
    mSeparator(separator),
    mTransmitter(submit)
{
    mFileSize = mInputSource.size();

    const auto alignment = file_type::alignment();
    assert(alignment);

    if (countPage) {
        mCapacityBlock = countPage;
    } else {
        auto countAlignmentPage = mFileSize / alignment;
        if (countAlignmentPage <= static_cast<ushort>(BlockSettings::CountBlockForSmallFile)) {
            mCapacityBlock = mFileSize;
            mCountBlocks = 1;
        }
        assert(countWorkers);
        mCapacityBlock = std::clamp(static_cast<ulong>(countAlignmentPage / countWorkers / 2),
                                         static_cast<ulong>(BlockSettings::MinimumRange),
                                         static_cast<ulong>(BlockSettings::MaximumRange));
    }
    mCapacityBlock *= alignment;
    std::cout << "File size = " << mFileSize
              << " [" << mCapacityBlock
              << "] count page = " << countPage  << std::endl;
}

FileSeparator::~FileSeparator()
{
    stop();
    std::cout << "dest FileSeparator" << std::endl;
}

size_t FileSeparator::countBlocks()
{
    return mCountBlocks;
}

size_t FileSeparator::start()
{
    mAlive = true;
    return separate();
}

void FileSeparator::stop()
{
    mAlive = false;
}

size_t FileSeparator::separate() const {
    auto begin = mInputSource.const_data();
    const auto end = begin + mInputSource.size();

    if (mCapacityBlock == mFileSize) {
        mTransmitter({begin, end});
        return 1;
    }

    size_t countBlock {0};
    while (begin < end && mAlive) {
        if (begin >= end - mCapacityBlock) {
            mTransmitter({begin, end});
            ++countBlock;
            break;
        }
        auto endBlock = begin + mCapacityBlock;
        while (endBlock > begin) {
            if (char_type::eq(*endBlock--, mSeparator.front())) {
                mTransmitter({begin, ++endBlock});
                ++countBlock;
                break;
            }
        }
        if (endBlock <= begin) {
            throw (std::length_error("Word size greater then "
                                     "allocation granularity for virtual memory"));
        }
        begin = ++endBlock;
    }
    return countBlock;
}

} // core
