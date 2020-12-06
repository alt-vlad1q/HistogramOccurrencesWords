#include "file-separator.hpp"

#include <cmath>
#include <exception>
#include <iostream>

FileSeparator::FileSeparator(file_type &file,
                             const std::string &separator,
                             submit_type submit,
                             ushort countPage) :
    mInputSource(file),
    mSeparator(separator),
    mTransmitter(submit)
{
    const auto granularity = static_cast<size_type>(file_type::alignment() * countPage);
    mFileSize = mInputSource.size();
    mCapacityBlock = mFileSize < granularity * 2 ? mFileSize : granularity;

    std::cout << "File size = " << mFileSize << " [" << mCapacityBlock << "]" << std::endl;
    separate();
}

void FileSeparator::separate() const {
    auto begin = mInputSource.const_data();
    const auto end = begin + mInputSource.size();

    if (mCapacityBlock == mFileSize) {
        mTransmitter({begin, end});
        return;
    }

    while (begin < end) {
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
