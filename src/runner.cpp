#include <iostream>
#include <getopt.h>
#include <cstdlib>
#include <optional>
#include <variant>
#include <charconv>
#include <string>
#include <cstdint>
#include "program_runner.hpp"


void ProgramRunner::print_error() {
    std::cerr << program_name << ": bad usage\n"
              << "Try '" << program_name << " --help' for more information.\n";
}

void ProgramRunner::print_help() {
    std::cout << "Usage: " << program_name << " [options]\n"
              << "Options:\n"
              << "  -h, --help           Show this help message\n"
              << "  -v, --version        Show version information\n"
              << "  -a, --algorithm      Specify the algorithm (default: xorshift)\n"
              << "  -m, --min            Minimum value\n"
              << "  -M, --max            Maximum value\n"
              << "  -c, --count          Number of random numbers to generate (default: 1)\n"
              << "  -t, --type           Specify the type to output (default: unit)\n";
}   

void ProgramRunner::print_version() {
    std::cout << program_name << " " << version << "\n";
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


template <typename T>
concept FromCharsParsable = std::is_integral_v<T> || std::is_floating_point_v<T>;


// Helper function for parsing a string to a numeric type using std::from_chars
template <FromCharsParsable T>
std::optional<T> parse_value(const std::string& str) {
    T value;
    auto result = std::from_chars(str.data(), str.data() + str.size(), value);
    if (result.ec != std::errc() || result.ptr != str.data() + str.size()) {
        return std::nullopt;
    }
    return value;
}

template <FromCharsParsable T>
std::optional<std::pair<T, T>> parse_min_and_max_numbers(const std::string &min_str, const std::string &max_str) {
    std::optional<T> min_value = parse_value<T>(min_str);
    std::optional<T> max_value = parse_value<T>(max_str);
    if (!min_value.has_value() || !max_value.has_value()) return std::nullopt;
    if (min_value.value() <= max_value.value()) return std::make_pair(min_value.value(), max_value.value());
    return std::nullopt;
}


void ProgramRunner::determine_generation_range_configuration(const std::optional<std::string> &min_str, const std::optional<std::string> &max_str){
    bool both_max_and_min_specified = min_str.has_value() && max_str.has_value();
    bool neither_max_or_min_specified = !min_str.has_value() && !max_str.has_value();
    if (both_max_and_min_specified && this->behaviour == ProgramBehaviour::GenerateInteger){
        auto min_and_max = parse_min_and_max_numbers<int64_t>(min_str.value(), max_str.value());
        if (!min_and_max.has_value()){
            this->behaviour = ProgramBehaviour::Error;
            return;
        }
        this->min = min_and_max.value().first;
        this->max = min_and_max.value().second;
    } else if (both_max_and_min_specified && this->behaviour == ProgramBehaviour::GenerateFloating){
        auto min_and_max = parse_min_and_max_numbers<double>(min_str.value(), max_str.value());
        if (!min_and_max.has_value()){
            this->behaviour = ProgramBehaviour::Error;
            return;
        }
        this->min = min_and_max.value().first;
        this->max = min_and_max.value().second;
    } else if (neither_max_or_min_specified && this->behaviour == ProgramBehaviour::GenerateUnitNormal){
        return;
    } else {
        this->behaviour = ProgramBehaviour::Error;
        return;
    }
}

void ProgramRunner::determine_user_message_configuration(const bool error, const bool show_version, const bool show_help){
    if (error){
        this->behaviour = ProgramBehaviour::Error;
        return;
    } else if (show_version){
        this->behaviour = ProgramBehaviour::Version;
        return;
    } else if (show_help){
        this->behaviour = ProgramBehaviour::Help;
        return;
    }
}

void ProgramRunner::determine_algorithm_configuration(const std::optional<std::string> &alg_str){
    if (!alg_str.has_value()){
        this->algorithm = Algorithm::XORShift; // default algorithm to use
    } else if (algorithm_choices.contains(alg_str.value())) { // TODO: this check/get can be made more efficient using find
        this->algorithm = algorithm_choices.at(alg_str.value());
    } else {
        this->behaviour = ProgramBehaviour::Error;
    }
}

void ProgramRunner::determine_generation_type_configuration(const std::optional<std::string> &generation_type){
    if (!generation_type.has_value()){
        this->behaviour = ProgramBehaviour::GenerateInteger;
    } else if (generation_types.contains(generation_type.value())){
        this->behaviour = generation_types.at(generation_type.value());
    } else {
        this->behaviour = ProgramBehaviour::Error;
    }
}

void ProgramRunner::determine_count_configuration(const std::optional<std::string> &count_str){
    if (count_str.has_value()){
        std::optional<uint32_t> count_val = parse_count_value(count_str.value());
        if (!count_val.has_value()){
            this->behaviour = ProgramBehaviour::Error;
            return;
        }
        this->count = count_val;
    } else {
        this->count = DefaultCount;
    }
}


void ProgramRunner::determine_program_configuration(const ProgramRunner::RawArguments raw_args){

    determine_user_message_configuration(raw_args.error, raw_args.show_version, raw_args.show_help);
    auto &behave = this->behaviour;
    if (behave.has_value() && (behave == ProgramBehaviour::Error || behave == ProgramBehaviour::Version || behave == ProgramBehaviour::Help)){
        return;
    }

    determine_algorithm_configuration(raw_args.algorithm_str);
    if (behave.has_value() && behave == ProgramBehaviour::Error){
        return;
    }

    determine_generation_type_configuration(raw_args.type);
    if (behave.has_value() && behave == ProgramBehaviour::Error){
        return;
    }

    determine_count_configuration(raw_args.count_str);
    if (behave.has_value() && behave == ProgramBehaviour::Error){
        return;
    }

    determine_generation_range_configuration(raw_args.min_str, raw_args.max_str);

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

ProgramRunner::ProgramRunner(int argc, char **argv){
    RawArguments raw_arguments = parse_args(argc, argv);
    determine_program_configuration(raw_arguments);
    
    if (this->behaviour.value() == ProgramBehaviour::Error){
        print_error();
        exit(1);
    } else if (this->behaviour.value() == ProgramBehaviour::Help){
        print_help();
        exit(0);
    } else if (this->behaviour.value() == ProgramBehaviour::Version){
        print_version();
        exit(0);
    } else {
        std::cout << "You got to another type of state!" << std::endl;
        exit(0);
    }

}
