#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <optional>
#include <variant>
#include <charconv>
#include <string>
#include <cstdint>
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
              << "  -t, --type           Specify the type to output\n"
              << "  -f, --float          Generate floating-point random numbers\n"
              << "  -i, --integer        Generate integer random numbers\n";
}   

void ProgramRunner::print_version() {
    std::cout << "Random Number Generator " << version << "\n";
}

bool ProgramRunner::erroneous_flag_args_provided(const ProgramRunner::RawArguments raw_arguments){

}



// Parses a string to uint32_t, returns std::nullopt on error or out-of-range
std::optional<uint32_t> parse_count_value(const std::string &count_str) {
    uint32_t value;
    const auto [ptr, ec] = std::from_chars(count_str.data(), count_str.data() + count_str.size(), value);
    if (ec != std::errc() || ptr != count_str.data() + count_str.size()) {
        return std::nullopt; // Parsing failed or extra characters present
    }
    return value;
}

std::optional<std::pair<int64_t, int64_t>> parse_min_and_max_integers(const std::string &min_str, const std::string &max_str) {
    int64_t min_value;
    const auto [ptr, ec] = std::from_chars(min_str.data(), min_str.data() + min_str.size(), min_value);
    if (ec != std::errc() || ptr != min_str.data() + min_str.size()) {
        return std::nullopt; // Parsing failed or extra characters present
    }

    int64_t max_value;
    const auto [ptr, ec] = std::from_chars(max_str.data(), max_str.data() + max_str.size(), max_value);
    if (ec != std::errc() || ptr != max_str.data() + max_str.size()) {
        return std::nullopt; // Parsing failed or extra characters present
    }

    if (min_value <= max_value) {
        std::pair<int64_t, int64_t> range = {min_value, max_value};
        std::optional<std::pair<int64_t, int64_t>> range_opt = range;
        return range_opt;
    }


    return std::nullopt;
}

std::optional<std::pair<double, double>> parse_min_and_max_doubles(const std::string &min_str, const std::string &max_str) {
    double min_value;
    const auto [ptr, ec] = std::from_chars(min_str.data(), min_str.data() + min_str.size(), min_value);
    if (ec != std::errc() || ptr != min_str.data() + min_str.size()) {
        return std::nullopt; // Parsing failed or extra characters present
    }

    double max_value;
    const auto [ptr, ec] = std::from_chars(max_str.data(), max_str.data() + max_str.size(), max_value);
    if (ec != std::errc() || ptr != max_str.data() + max_str.size()) {
        return std::nullopt; // Parsing failed or extra characters present
    }

    if (min_value <= max_value) {
        std::pair<double, double> range = {min_value, max_value};
        std::optional<std::pair<double, double>> range_opt = range;
        return range_opt;
    }


    return std::nullopt;
}

void ProgramRunner::determine_program_configuration(const ProgramRunner::RawArguments raw_args){
    if (raw_args.error){
        this->behaviour = ProgramBehaviour::Error;
        return;
    } else if (raw_args.show_version){
        this->behaviour = ProgramBehaviour::Version;
        return;
    } 
    
     if (!raw_args.algorithm_str.has_value()){
        this->algorithm = Algorithm::XORShift; // default algorithm to use
     } else if (algorithm_choices.contains(raw_args.algorithm_str.value())) { // TODO: this check/get can be made more efficient using find
        this->algorithm = algorithm_choices.at(raw_args.algorithm_str.value());
     } else {
        this->behaviour = ProgramBehaviour::Error;
        return;
     }

     if (!raw_args.type.has_value()){
        this->behaviour = ProgramBehaviour::GenerateInteger;
     } else if (generation_types.contains(raw_args.type.value())){
        this->behaviour = generation_types.at(raw_args.type.value());
     } else {
        this->behaviour = ProgramBehaviour::Error;
     }

     if (raw_args.count_str.has_value()){
        std::optional<uint32_t> count_val = parse_count_value(raw_args.count_str.value());
        if (!count_val.has_value()){
            this->behaviour = ProgramBehaviour::Error;
            return;
        }
        this->count = count_val;
     } else {
        this->count = DefaultCount;
     }



     bool both_max_and_min_specified = raw_args.min_str.has_value() && raw_args.max_str.has_value();
     bool neither_max_or_min_specified = !raw_args.min_str.has_value() && !raw_args.max_str.has_value();
     if (both_max_and_min_specified && this->behaviour == ProgramBehaviour::GenerateInteger){
        auto min_and_max = parse_min_and_max_integers(raw_args.min_str.value(), raw_args.max_str.value());
        if (!min_and_max.has_value()){
            this->behaviour = ProgramBehaviour::Error;
            return;
        }
        this->min = min_and_max.value().first;
        this->max = min_and_max.value().second;
     } else if (both_max_and_min_specified && this->behaviour == ProgramBehaviour::GenerateFloating){
        auto min_and_max = parse_min_and_max_doubles(raw_args.min_str.value(), raw_args.max_str.value());
        if (!min_and_max.has_value()){
            this->behaviour = ProgramBehaviour::Error;
            return;
        }
        this->min = min_and_max.value().first;
        this->max = min_and_max.value().second;
     } else if (neither_max_or_min_specified && this->behaviour == ProgramBehaviour::GenerateUnitNormal){
        this->min = DefaultMin;
        this->max = DefaultMax;
     } else {
        this->behaviour = ProgramBehaviour::Error;
        return;
     }

     return;
}

ProgramRunner::RawArguments ProgramRunner::parse_args(int argc, char **argv) {
    const char* const short_opts = "hva:m:M:c:t:";
    const ::option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"version", no_argument, nullptr, 'v'},
        {"algorithm", required_argument, nullptr, 'a'},
        {"min", required_argument, nullptr, 'm'},
        {"max", required_argument, nullptr, 'M'},
        {"count", required_argument, nullptr, 'c'},
        {"type", no_argument, nullptr, 't'},
        {nullptr, 0, nullptr, 0}
    };

    RawArguments raw_arguments;

    raw_arguments.error = false;
    raw_arguments.show_help = false;
    raw_arguments.show_version = false;
    raw_arguments.algorithm_str = std::nullopt;
    raw_arguments.min_str = std::nullopt;
    raw_arguments.max_str = std::nullopt;
    raw_arguments.count_str = std::nullopt;
    raw_arguments.type = std::nullopt;

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
            case 't':
                raw_arguments.type = std::string(optarg);
                break;
            case '?': // Unrecognized option
                raw_arguments.error = true;
                return raw_arguments;
            default:
                raw_arguments.error = true;
                return raw_arguments;
        }
    }

    return raw_arguments;
}
