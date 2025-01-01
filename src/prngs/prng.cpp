#include <chrono>
#include <cstdint>
#include "prng.hpp"

PseudoRandomNumberGenerator::PseudoRandomNumberGenerator() {
    this->seed = generateCryptographicallyInsecureSeed();
};

PseudoRandomNumberGenerator::PseudoRandomNumberGenerator(const std::uint64_t seed) {
    this->seed = seed;
};

std::uint64_t PseudoRandomNumberGenerator::generateCryptographicallyInsecureSeed() {
    auto current_time = std::chrono::system_clock::now();
    auto current_time_duration = current_time.time_since_epoch(); // convert a bare time to a duration
    auto time_as_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time_duration); // get the duration as a value in milliseconds
    std::uint64_t seed_from_milliseconds = time_as_milliseconds.count(); // convert the milliseconds to a bare integer and use that as our seed
    return seed_from_milliseconds;
};
