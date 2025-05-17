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


    enum class Algorithm {
        XORShift,
        LinearCongruentialGenerator,
        // Add other algorithms here
    };



    enum class ProgramBehaviour {
        Error,
        Help,
        Version,
        GenerateUnitNormal,
        GenerateFloating,
        GenerateInteger
    };

    struct RawArguments {
        bool error;
        bool show_help;
        bool show_version;
        bool unit;
        bool floating;
        bool integer;
        std::string algorithm_str;
        std::string min_str;
        std::string max_str;
        std::string count_str;
    };

    RawArguments parse_args(int argc, char **argv);
    void print_help();
    void print_version();

    ProgramBehaviour option;
    std::optional<Algorithm> algorithm;
    std::optional<std::variant<std::uint64_t, std::int64_t, double>> min;
    std::optional<std::variant<std::uint64_t, std::int64_t, double>> max;
    std::optional<std::uint64_t> count;




};