#pragma once

#include <boost/program_options.hpp>

namespace detail {

namespace po = boost::program_options;

class OptionsHandler
{
    po::options_description desc;
    po::variables_map vm;

public:
    OptionsHandler(int &argc, char **argv);

    void printHelp () const noexcept;

    std::string filePath;
    unsigned short countPage;
    bool oneThread;
};

} //detail
