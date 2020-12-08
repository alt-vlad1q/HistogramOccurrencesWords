#include "file-separator.hpp"

#include <cmath>
#include <exception>
#include <iostream>
#include <thread>

namespace core {

FileSeparator::FileSeparator(file_type &file,
                             const std::string &separator,
                             submit_type submit,
                             ushort countWorkers,
                             ushort countPage) :
    mAlive(true),
    mInputSource(file),
    mCountBlocks(0),
    mSeparator(separator),
    mTransmitter(submit)
{
    preparationFile(countWorkers, countPage);
    preparationBounds();
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

void FileSeparator::start()
{
    mAlive = true;
    separate();
}

void FileSeparator::stop()
{
    mCountBlocks = 0;
    mAlive = false;
}

void FileSeparator::preparationFile(ushort countWorkers, ushort countPage)
{
    mFileSize = mInputSource.size();
    const auto alignment = file_type::alignment();
    assert(alignment);

    if (countPage) {
        mCapacityBlock = countPage * alignment;
    } else {
        auto countAlignmentPage = mFileSize / alignment;
        if (countAlignmentPage <= static_cast<ushort>(BlockSettings::CountBlockForSmallFile)) {
            mCapacityBlock = mFileSize;
        } else {
            assert(countWorkers);
            mCapacityBlock = std::clamp(static_cast<ulong>(countAlignmentPage / countWorkers / 2),
                                        static_cast<ulong>(BlockSettings::MinimumRange),
                                        static_cast<ulong>(BlockSettings::MaximumRange));
            mCapacityBlock *= alignment;
        }
    }

    std::cout << "File size = " << mFileSize
              << " [" << mCapacityBlock
              << "] count page = " << countPage  << std::endl;
}

void FileSeparator::preparationBounds()
{
    auto begin = mInputSource.const_data();
    const auto end = begin + mInputSource.size();

    if (mCapacityBlock == mFileSize) {
        ++mCountBlocks;
        return;
    }

    while (begin < end && mAlive) {
        if (begin >= end - mCapacityBlock) {
            ++mCountBlocks;
            break;
        }
        auto endBlock = begin + mCapacityBlock;
        while (endBlock > begin) {
            if (char_type::eq(*endBlock--, mSeparator.front())) {
                ++endBlock;
                ++mCountBlocks;
                break;
            }
        }
        if (endBlock <= begin) {
            throw (std::length_error("Word size greater then "
                                     "allocation granularity for virtual memory"));
        }
        begin = ++endBlock;
    }
}

void FileSeparator::separate() const
{
    auto begin = mInputSource.const_data();
    const auto end = begin + mInputSource.size();

    if (mCapacityBlock == mFileSize) {
        mTransmitter({begin, end});
        return;
    }

    while (begin < end && mAlive) {
        if (begin >= end - mCapacityBlock) {
            mTransmitter({begin, end});
            break;
        }
        auto endBlock = begin + mCapacityBlock;
        while (endBlock > begin) {
            if (char_type::eq(*endBlock--, mSeparator.front())) {
                mTransmitter({begin, ++endBlock});
                break;
            }
        }
        if (endBlock <= begin) {
            throw (std::length_error("Word size greater then "
                                     "allocation granularity for virtual memory"));
        }
        begin = ++endBlock;
    }
}

} // core
