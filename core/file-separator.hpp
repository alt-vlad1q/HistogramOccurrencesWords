#pragma once

#include <memory>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/code_converter.hpp>

class FileSeparator {
public:
    using file_type = boost::iostreams::mapped_file;
    using size_type = file_type::size_type;
    using char_type = std::char_traits<file_type::char_type>;
    using submit_type = std::function<void(const std::string &)>;

    FileSeparator (file_type &file,
                   const std::string &separator,
                   submit_type submit,
                   ushort countPage = 1);

private:
    void separate () const;

private:
    file_type &mInputSource;
    size_type mFileSize;
    std::string mSeparator;
    submit_type mTransmitter;
    size_type mCapacityBlock;
};
