#include <gtest/gtest.h>
#include "program_runner.hpp"

constexpr const char* highest_i32_plus_one = "2147483648";
constexpr const char* lowest_i32_minus_one = "-2147483649";
constexpr const char* highest_u32_plus_one = "4294967296";

struct ArgvBuilder {
    std::vector<std::string> args;
    char **argv; 

    ArgvBuilder(std::initializer_list<std::string> init) : args(init) {
        std::string program_name = "this-should-not-matter";
        args.emplace(args.begin(), program_name);
        size_t null_terminator_arg_element = 1;
        size_t all_args_size = args.size() + null_terminator_arg_element;
        argv = new char*[all_args_size]; // NOLINT (cppcoreguidelines-owning-memory)

        for (size_t i = 0; i < args.size(); ++i){
            std::string this_arg = args[i];
            size_t null_terminated_size = this_arg.size() + 1;
            argv[i] = new char[null_terminated_size]; // NOLINT (cppcoreguidelines-owning-memory)
            strncpy(argv[i], this_arg.c_str(), null_terminated_size);
            argv[i][this_arg.size()] = '\0';
        }

        argv[args.size()] = nullptr;
    }

    ArgvBuilder(const ArgvBuilder& other) = delete;
    ArgvBuilder(ArgvBuilder&& other) = delete;
    ArgvBuilder& operator=(const ArgvBuilder& other) = delete;
    ArgvBuilder& operator=(const ArgvBuilder&& other) = delete;

    ~ArgvBuilder() {
        for (size_t i = 0; i < args.size(); i++){
            delete[] argv[i]; // NOLINT (cppcoreguidelines-owning-memory)
        }
        delete[] argv;
    }

    int argc() const { return static_cast<int>(args.size()); }
    char** argv_ptr() const { return argv; }
};

TEST(TestProgramRunner, NoArgs){
    ArgvBuilder builder({});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code, 0);
}

TEST(TestProgramRunner, JustHelp){
    ArgvBuilder builder({"--help"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code, 0);
}

TEST(TestProgramRunner, JustVersion){
    ArgvBuilder builder({"--version"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code, 0);
}

TEST(TestProgramRunner, HelpThenVersion){
    ArgvBuilder builder({"--help", "--version"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, VersionThenHelp){
    ArgvBuilder builder({"--version", "--help"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, HelpThenOther){
    ArgvBuilder builder({"--help", "other", "stuff", "which", "is", "ignored"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, VersionThenOther){
    ArgvBuilder builder({"--version", "ignore", "this", "stuff", "please", "and", "thank", "you"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, BadSubcommands){
    ArgvBuilder builder({"badsubcommand", "--help"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, DefaultAlgorithm){
    ArgvBuilder builder({"--min", "0", "--max=10", "-c", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitAlgorithmXOR){
    ArgvBuilder builder({"--algorithm", "xor", "-m", "0", "-M10", "--count", "5", "-t", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitAlgorithmLCG){
    ArgvBuilder builder({"--algorithm=linear-congruential-generator", "-m0", "-M", "10", "--count=5", "-tint"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitAlgorithmMersenne){
    ArgvBuilder builder({"-a", "mersenne", "-m", "0", "-M", "10", "--count=5", "--type=int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitAlgorithmInvalid){
    ArgvBuilder builder({"--algorithm", "invalid-algorithm", "--min", "0", "-M", "10", "--count", "5", "-t", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, NoMinOrMax){
    ArgvBuilder builder({"--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, MinNoMax){
    ArgvBuilder builder({"--min", "0", "--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, MaxNoMin){
    ArgvBuilder builder({"-M", "11.1", "--count", "5", "--type", "float"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, MinAndMax){
    ArgvBuilder builder({"--min", "0", "--max", "6", "--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, MinExcessiveNegativeMagnitude){
    ArgvBuilder builder({"--min", lowest_i32_minus_one, "--max", "10", "--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, MinExcessivePositiveMagnitude){
    ArgvBuilder builder({"--min", highest_i32_plus_one, "--max", "10", "--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, MinAndMaxExcessiveNegativeMagnitude){
    ArgvBuilder builder({"--min", lowest_i32_minus_one, "--max", lowest_i32_minus_one, "--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, MinAndMaxExcessivePositiveMagnitude){
    ArgvBuilder builder({"--min", highest_i32_plus_one, "--max", highest_i32_plus_one, "--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, DefaultCount){
    ArgvBuilder builder({"--min", "0", "--max", "10", "--type", "floating"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.iterate();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitCount){
    int explicit_count = 34539; // Arbitrary positive count for testing
    int all_but_last = explicit_count - 1;
    std::string explicit_count_str = std::to_string(explicit_count);
    ArgvBuilder builder({"--min", "0", "--max", "10", "--count", explicit_count_str, "--type", "integer"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    for (int i = 0; i < all_but_last; ++i) {
        ProgramRunner::ProgramStatus status = program_runner.iterate();
        ASSERT_FALSE(status.stderr_message.has_value());
        ASSERT_TRUE(status.stdout_message.has_value());
        ASSERT_FALSE(status.exit_code.has_value());
    }

    ProgramRunner::ProgramStatus status = program_runner.iterate();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, CountIsZero){
    ArgvBuilder builder({"--min", "0", "--max", "10", "--count", "0", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.iterate();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, CountIsOne){
    ArgvBuilder builder({"--count", "1", "--type", "unit"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.iterate();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, CountIsNegative){
    ArgvBuilder builder({"--min", "0", "--max", "10", "--count", "-5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.iterate();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, CountExcessivePositiveMagnitude){
    ArgvBuilder builder({"--min", "0", "--max", "10", "--count", highest_u32_plus_one, "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.iterate();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, DefaultType){
    ArgvBuilder builder({"-m0", "-M1", "--count", "5", "--algorithm", "xorshift"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitTypeUnit){
    ArgvBuilder builder({"--count", "5", "--type", "unit"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitTypeFloat){
    ArgvBuilder builder({"--min", "-4.3", "--max", "10.5", "--count", "5", "--type", "float"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitTypeInt){
    ArgvBuilder builder({"--min", "0", "--max", "10", "--count", "5", "--type", "int"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_FALSE(status.stderr_message.has_value());
    ASSERT_TRUE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_EQ(status.exit_code.value(), 0);
}

TEST(TestProgramRunner, ExplicitTypeInvalid){
    ArgvBuilder builder({"--min", "0", "--max", "10", "--count", "5", "--type", "invalid"});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
    ProgramRunner::ProgramStatus status = program_runner.run();
    ASSERT_TRUE(status.stderr_message.has_value());
    ASSERT_FALSE(status.stdout_message.has_value());
    ASSERT_TRUE(status.exit_code.has_value());
    ASSERT_NE(status.exit_code.value(), 0);
}


