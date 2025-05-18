#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <optional>
#include <variant>
#include <map>

class ProgramRunner {   
public:
    ProgramRunner(int argc, char **argv);

    void iterate();
    void iterate(std::uint64_t iterations);
    void run();

    const std::string version = "0.1";
    const std::string program_name = "randomizer";

private:

    // Defaults

    static constexpr uint32_t DefaultCount = 1;
    static constexpr int64_t DefaultMin = 1;
    static constexpr int64_t DefaultMax = 6;

    enum class Algorithm {
        XORShift,
        LinearCongruentialGenerator,
        // Add other algorithms here
    };

    const std::map<std::string, Algorithm> algorithm_choices {
        {"xorshift", Algorithm::XORShift},
        {"xor", Algorithm::XORShift},
        {"linear-congruential-generator", Algorithm::LinearCongruentialGenerator},
        {"lcg", Algorithm::LinearCongruentialGenerator}
    };



    enum class ProgramBehaviour {
        Error,
        Help,
        Version,
        GenerateUnitNormal,
        GenerateFloating,
        GenerateInteger
    };

    const std::map<std::string, ProgramBehaviour> generation_types {
        {"unit", ProgramBehaviour::GenerateUnitNormal},
        {"normal", ProgramBehaviour::GenerateUnitNormal},
        {"unit-normal", ProgramBehaviour::GenerateUnitNormal},
        {"normalized", ProgramBehaviour::GenerateUnitNormal},
        {"float", ProgramBehaviour::GenerateFloating},
        {"floating", ProgramBehaviour::GenerateFloating},
        {"decimal", ProgramBehaviour::GenerateFloating},
        {"floating-point", ProgramBehaviour::GenerateFloating},
        {"int", ProgramBehaviour::GenerateInteger},
        {"integer", ProgramBehaviour::GenerateInteger}
    };

    struct RawArguments {
        // what special strings should we show?

        bool error;
        bool show_help;
        bool show_version;

        // how do we want to generate the random numbers?
        std::optional<std::string> type;

        std::optional<std::string> algorithm_str;
        std::optional<std::string> min_str;
        std::optional<std::string> max_str;
        std::optional<std::string> count_str;
    };

    RawArguments parse_args(int argc, char **argv);
    void determine_program_configuration(const RawArguments raw_Arguments);
    void determine_generation_configuration(const std::optional<std::string> &min_str, const std::optional<std::string> &max_str);

    bool erroneous_flag_args_provided(const RawArguments raw_Arguments);
    void print_help();
    void print_version();

    ProgramBehaviour behaviour;
    std::optional<Algorithm> algorithm;
    std::optional<std::variant<int64_t, double>> min;
    std::optional<std::variant<int64_t, double>> max;
    std::optional<uint32_t> count;


};