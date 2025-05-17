#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <optional>
#include <variant>
#include "program_runner.hpp"

void ProgramRunner::print_help() {
    std::cout << "Usage: " << program_name << " [options]\n"
              << "Options:\n"
              << "  -h, --help           Show this help message\n"
              << "  -v, --version        Show version information\n"
              << "  -a, --algorithm      Specify the algorithm (default: xorshift)\n"
              << "  -m, --min            Minimum value\n"
              << "  -M, --max            Maximum value\n"
              << "  -c, --count          Number of random numbers to generate\n"
              << "  -u, --unit           Generate unit normal random numbers\n"
              << "  -f, --float          Generate floating-point random numbers\n"
              << "  -i, --integer        Generate integer random numbers\n";
}   

void ProgramRunner::print_version() {
    std::cout << "Random Number Generator " << version << "\n";
}


ProgramRunner::RawArguments ProgramRunner::parse_args(int argc, char **argv) {
    const char* const short_opts = "hva:m:M:c:ufi";
    const ::option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"version", no_argument, nullptr, 'v'},
        {"algorithm", required_argument, nullptr, 'a'},
        {"min", required_argument, nullptr, 'm'},
        {"max", required_argument, nullptr, 'M'},
        {"count", required_argument, nullptr, 'c'},
        {"unit", no_argument, nullptr, 'u'},
        {"float", no_argument, nullptr, 'f'},
        {"integer", no_argument, nullptr, 'i'},
        {nullptr, 0, nullptr, 0}
    };

    RawArguments raw_arguments;

    raw_arguments.error = false;
    raw_arguments.show_help = false;
    raw_arguments.show_version = false;

    raw_arguments.algorithm_str = "xorshift"; // Default algorithm

    // Standard defaults if a user does not provide a range they want to use, will roll a dice. 
    raw_arguments.min_str = "1";
    raw_arguments.max_str = "6";

    raw_arguments.count_str = "1";

    // By default we will be rolling a dice, so we will be generating integers. 
    raw_arguments.unit = false;
    raw_arguments.floating = false;
    raw_arguments.integer = true;

    while (true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (opt == -1)
            break;

        switch (opt) {
            case 'h':
                raw_arguments.show_help = true;
                break;
            case 'v':
                raw_arguments.show_version = true;
                break;
            case 'a':
                raw_arguments.algorithm_str = std::string(optarg);
                break;
            case 'm':
                raw_arguments.min_str = std::string(optarg);
                break;
            case 'M':   
                raw_arguments.max_str = std::string(optarg);
                break;
            case 'c':
                raw_arguments.count_str = std::string(optarg);
                break;
            case 'u':
                raw_arguments.unit = true;
                break;
            case 'f':
                raw_arguments.floating = true;
                break;
            case 'i':   
                raw_arguments.integer = true; 
                break;
            case '?': // Unrecognized option
                raw_arguments.error = true;
                exit(1);
            default:
                raw_arguments.error = true;
                exit(1);
        }
    }

    return raw_arguments;
}
