#pragma once

#include <memory>

#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/code_converter.hpp>

namespace core {

class FileSeparator {

    enum class BlockSettings {
        MinimumRange = 1,
        MaximumRange = 200,
        CountBlockForSmallFile = 2
    };

public:
    using file_type = boost::iostreams::mapped_file;
    using size_type = file_type::size_type;
    using char_type = std::char_traits<file_type::char_type>;
    using submit_type = std::function<void(const std::string &)>;

    FileSeparator (file_type &file,
                   const std::string &separator,
                   submit_type submit,
                   ushort countWorkers,
                   ushort countPage);
    ~FileSeparator();

    size_t countBlocks ();
    void start();
    void stop ();

private:
    void preparationFile(ushort countWorkers, ushort countPage);
    void preparationBounds();
    void separate() const;

private:
    std::atomic_bool mAlive;
    file_type &mInputSource;
    size_type mFileSize;
    size_t mCountBlocks;


    std::string mSeparator;
    submit_type mTransmitter;
    size_type mCapacityBlock;
};

} // core
