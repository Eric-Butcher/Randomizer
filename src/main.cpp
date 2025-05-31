#include <iostream>
#include <getopt.h>
#include "prng.hpp"
#include "linear_congruential_generator.hpp"
#include "xorshift.hpp"
#include "program_runner.hpp"

int main(int argc, char *argv[]){
    ProgramRunner runner = ProgramRunner(argc, argv);
    ProgramRunner::ProgramStatus status = runner.run();
    return 0;
}