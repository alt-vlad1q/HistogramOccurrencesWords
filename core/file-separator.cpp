#include "file-separator.hpp"

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
    separate(true);
}

FileSeparator::~FileSeparator()
{
    stop();
}

size_t FileSeparator::countBlocks() const
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
            mCapacityBlock = std::clamp(static_cast<unsigned long>(countAlignmentPage /
                                                                   countWorkers / 2),
                                        static_cast<unsigned long>(BlockSettings::MinimumRange),
                                        static_cast<unsigned long>(BlockSettings::MaximumRange));
            mCapacityBlock *= alignment;
        }
    }
}

void FileSeparator::separate(bool preparation)
{
    auto begin = mInputSource.const_data();
    const auto end = begin + mInputSource.size();

    if (mCapacityBlock == mFileSize) {
        if(preparation)
            ++mCountBlocks;
        else
            mTransmitter({begin, end});
        return;
    }

    while (begin < end && mAlive) {
        if (begin >= end - mCapacityBlock) {
            if(preparation)
                ++mCountBlocks;
            else
                mTransmitter({begin, end});
            break;
        }
        auto endBlock = begin + mCapacityBlock;
        while (endBlock > begin) {
            if (char_type::eq(*endBlock--, mSeparator.front())) {
                ++endBlock;
                if(preparation)
                    ++mCountBlocks;
                else
                    mTransmitter({begin, ++endBlock});
                break;
            }
        }
        if (endBlock <= begin) {
            throw (std::length_error("Word size greater then block size"));
        }
        begin = ++endBlock;
    }
}

} // core
