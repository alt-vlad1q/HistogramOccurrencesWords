#include "options-handler.hpp"

#include <iostream>

namespace detail {

OptionsHandler::OptionsHandler(int &argc, char **argv) :
    desc("Allowed options")
{
    desc.add_options()
        ("help,h", "produce help message")
        ("file,f", po::value<std::string>()->default_value(""), "path to the source file")
        ("singlethread", po::bool_switch()->default_value(false),
         "run main work of applications in one thread")
        ("countpage,c", po::value<unsigned short>()->default_value(0),
         "count page for allocation granularity for virtual memory")
    ;

    po::store(po::parse_command_line(argc, argv, desc), vm);

    try {
        po::notify(vm);
    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        printHelp();
    }

    filePath = vm["file"].as<std::string>();
    countPage = vm["countpage"].as<unsigned short>();
    singleThread = vm["singlethread"].as<bool>();
}

void OptionsHandler::printHelp() const noexcept
{
    std::cerr << desc << std::endl;
}

} // detail
