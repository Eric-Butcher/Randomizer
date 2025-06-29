#include <chrono>
#include <cstdint>
#include <random>
#include <limits>
#include "prng.hpp"

/* The most default constructor will create a random seed from the system time
and assume that the range of the PRNG is the full range of the unsigned 64-bit 
integer. While this is the only sensible default option, it is dangerous as it is
not generally true and will likely have the PRNG produce highly incorrect values when
generating uniform random floats. Use with caution! */
PseudoRandomNumberGenerator::PseudoRandomNumberGenerator() 
    : m_seed(generateCryptographicallyInsecureSeed()),
    m_minimum_value(std::numeric_limits<uint64_t>::min()),
    m_maximum_value(std::numeric_limits<uint64_t>::max()) {}


PseudoRandomNumberGenerator::PseudoRandomNumberGenerator(const uint64_t seed)
    : m_seed(seed),
    m_minimum_value(std::numeric_limits<uint64_t>::min()),
    m_maximum_value(std::numeric_limits<uint64_t>::max()) {}

PseudoRandomNumberGenerator::PseudoRandomNumberGenerator(const uint64_t minimum_value, const uint64_t maximum_value)
    : m_seed(generateCryptographicallyInsecureSeed()),
    m_minimum_value(minimum_value),
    m_maximum_value(maximum_value) {}


PseudoRandomNumberGenerator::PseudoRandomNumberGenerator(const uint64_t seed, const uint64_t minimum_value, const uint64_t maximum_value)
    : m_seed(seed),
    m_minimum_value(minimum_value),
    m_maximum_value(maximum_value) {}

std::uint64_t PseudoRandomNumberGenerator::generateCryptographicallyInsecureSeed() {
    auto current_time = std::chrono::system_clock::now();
    auto current_time_duration = current_time.time_since_epoch(); // convert a bare time to a duration
    auto time_as_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(current_time_duration); // get the duration as a value in milliseconds
    uint64_t seed_from_milliseconds = time_as_milliseconds.count(); // convert the milliseconds to a bare integer and use that as our seed
    return seed_from_milliseconds;
};

double PseudoRandomNumberGenerator::generateFloatingPointRandomValue(float min, float max) {
    double random_val = generateUnitNormalRandomValue();
    double scaled_value = min + (random_val * (max - min));
    return scaled_value;
}

int64_t PseudoRandomNumberGenerator::generateIntegerRandomValue(int32_t min, int32_t max) {
    double random_val = generateUnitNormalRandomValue();
    int64_t inclusive_range = max - min + 1;
    double random_val_magnitude = random_val * static_cast<double>(inclusive_range);
    int64_t scaled_value = static_cast<int64_t>(random_val_magnitude) + min;
    return scaled_value;
}

// Generate a random value normalized to the range [0, 1)
double PseudoRandomNumberGenerator::generateUnitNormalRandomValue() {
  
    uint64_t random_value = generateRandomValue();

    uint64_t range = m_maximum_value - m_minimum_value;
    if (range == 0) {
        return 0.0; // Avoid division by zero
    }

    // Normalize the random value to [0, 1)
    double normalized_value = static_cast<double>(random_value - m_minimum_value) / static_cast<double>(range);
    return normalized_value;
}