#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <optional>
#include <variant>
#include <map>
#include <memory>
#include "prng.hpp"

class ProgramRunner {   
public:
    ProgramRunner(int argc, char **argv);

    struct ProgramStatus {
        std::optional<std::string> stdout_message;
        std::optional<std::string> stderr_message;
        std::optional<int> exit_code;
    };

    ProgramStatus iterate();

    // will return the ProgramStatus of the last iteration
    ProgramStatus iterate(uint64_t iterations);

    // runs until the program runner finishes, will return the ProgramStatus of the last iteration
    ProgramStatus run();

    bool is_finished();

    const std::string version = "0.1";
    const std::string program_name = "randomizer";

    

private:

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

    
    // Defaults

    static constexpr uint32_t DefaultCount = 1;
    static constexpr ProgramBehaviour DefaultGenerationType = ProgramBehaviour::GenerateInteger;

    RawArguments parse_args(int argc, char **argv);
    void determine_program_configuration(const RawArguments &raw_arguments);
    void determine_user_message_configuration(const bool error, const bool show_version, const bool show_help);
    void determine_generation_range_configuration(const std::optional<std::string> &min_str, const std::optional<std::string> &max_str);
    void determine_generation_type_configuration(const std::optional<std::string> &generation_type);
    void determine_count_configuration(const std::optional<std::string> &count_str);
    void determine_algorithm_configuration(const std::optional<std::string> &alg_str);
    void create_prng();

    std::string error_string();
    std::string help_string();
    std::string version_string();

    std::optional<ProgramBehaviour> behaviour = std::nullopt;
    std::optional<Algorithm> algorithm = std::nullopt;
    std::optional<std::variant<int32_t, float>> min = std::nullopt;
    std::optional<std::variant<int32_t, float>> max = std::nullopt;
    std::optional<uint32_t> count = std::nullopt;

    uint32_t iteration = 0;
    bool finished = false;

    std::unique_ptr<PseudoRandomNumberGenerator> prng = nullptr;

    // Exit codes
    static constexpr int ExitCodeSuccess = 0;
    static constexpr int ExitCodeError = 1;



};