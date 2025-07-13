#include "linear_congruential_generator.hpp"
#include "prng.hpp"
#include "program_runner.hpp"
#include "xorshift.hpp"
#include <getopt.h>
#include <iostream>

int main(int argc, char* argv[])
{
    ProgramRunner runner = ProgramRunner(argc, argv, 1000);
    ProgramRunner::ProgramStatus status;
    while (!runner.is_finished()) {
        status = runner.iterate();
        if (status.stdout_message.has_value()) {
            std::cout << status.stdout_message.value() << std::endl;
        }
        if (status.stderr_message.has_value()) {
            std::cerr << status.stderr_message.value() << std::endl;
        }
        if (status.exit_code.has_value()) {
            exit(status.exit_code.value());
        }
    }
    exit(1); // If we reach here, something went wrong
}