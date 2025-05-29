#include <gtest/gtest.h>
#include "program_runner.hpp"

struct ArgvBuilder {
    std::vector<std::string> args;
    std::vector<char*> argv;

    ArgvBuilder(std::initializer_list<std::string> init) : args(init) {
        std::string program_name = "randomizer";
        argv.push_back(const_cast<char*>(program_name.c_str()));
        for (auto& s : args) {
            argv.push_back(const_cast<char*>(s.c_str()));
        }
        argv.push_back(nullptr); // argv must be null-terminated
    }

    int argc() const { return static_cast<int>(args.size()); }
    char** argv_ptr() { return argv.data(); }
};

TEST(TestProgramRunner, NoArgs){
    ArgvBuilder builder({});
    ProgramRunner program_runner = ProgramRunner(builder.argc(), builder.argv_ptr());
}

TEST(TestProgramRunner, JustHelp){

}

TEST(TestProgramRunner, JustVersion){

}

TEST(TestProgramRunner, HelpThenVersion){

}

TEST(TestProgramRunner, VersionThenHelp){

}

TEST(TestProgramRunner, HelpThenOther){

}

TEST(TestProgramRunner, VersionThenOther){

}

TEST(TestProgramRunner, BadSubcommands){

}

TEST(TestProgramRunner, DefaultAlgorithm){

}

TEST(TestProgramRunner, ExplicitAlgorithmXOR){

}

TEST(TestProgramRunner, ExplicitAlgorithmLCG){

}

TEST(TestProgramRunner, ExplicitAlgorithmMersenne){

}

TEST(TestProgramRunner, ExplicitAlgorithmInvalid){

}

TEST(TestProgramRunner, NoMinOrMax){

}

TEST(TestProgramRunner, MinNoMax){

}

TEST(TestProgramRunner, MaxNoMin){

}

TEST(TestProgramRunner, MinAndMax){

}

TEST(TestProgramRunner, MinExcessiveNegativeMagnitude){

}

TEST(TestProgramRunner, MinExcessivePositiveMagnitude){

}

TEST(TestProgramRunner, MinAndMaxExcessiveNegativeMagnitude){

}

TEST(TestProgramRunner, MinAndMaxExcessivePositiveMagnitude){

}

TEST(TestProgramRunner, DefaultCount){

}

TEST(TestProgramRunner, ExplicitCount){

}

TEST(TestProgramRunner, CountIsZero){

}

TEST(TestProgramRunner, CountIsOne){

}

TEST(TestProgramRunner, CountIsNegative){

}

TEST(TestProgramRunner, CountExcessivePositiveMagnitude){

}

TEST(TestProgramRunner, DefaultType){

}

TEST(TestProgramRunner, ExplicitTypeUnit){

}

TEST(TestProgramRunner, ExplicitTypeFloat){

}

TEST(TestProgramRunner, ExplicitTypeInt){

}

TEST(TestProgramRunner, ExplicitTypeInvalid){

}


