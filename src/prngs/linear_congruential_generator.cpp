#include <cstdint>
#include <algorithm>
#include "linear_congruential_generator.hpp"
#include "prng.hpp"


// Default constructor
LinearCongruentialGenerator::LinearCongruentialGenerator()
    : PseudoRandomNumberGenerator(getMinimumValue(DefaultMask), getMaximumValue(DefaultModulus, DefaultMask)),
      m_modulus(DefaultModulus),
      m_multiplier(DefaultMultiplier),
      m_increment(DefaultIncrement),
      m_mask(DefaultMask),
      m_current_value(m_seed) {}

// Constructor with seed
LinearCongruentialGenerator::LinearCongruentialGenerator(const std::uint64_t seed)
    : PseudoRandomNumberGenerator(seed, getMinimumValue(DefaultMask), getMaximumValue(DefaultModulus, DefaultMask)),
      m_modulus(DefaultModulus),
      m_multiplier(DefaultMultiplier),
      m_increment(DefaultIncrement),
      m_mask(DefaultMask),
      m_current_value(m_seed) {}

// Constructor with custom parameters
LinearCongruentialGenerator::LinearCongruentialGenerator(std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
    : PseudoRandomNumberGenerator(getMinimumValue(mask), getMaximumValue(modulus, mask)),
      m_modulus(modulus),
      m_multiplier(multiplier),
      m_increment(increment),
      m_mask(mask),
      m_current_value(m_seed) {}

// Constructor with seed and custom parameters
LinearCongruentialGenerator::LinearCongruentialGenerator(std::uint64_t seed, std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
    : PseudoRandomNumberGenerator(seed, getMinimumValue(mask), getMaximumValue(modulus, mask)),
      m_modulus(modulus),
      m_multiplier(multiplier),
      m_increment(increment),
      m_mask(mask),
      m_current_value(m_seed) {}


std::uint64_t LinearCongruentialGenerator::getMinimumValue(const std::uint64_t mask) {

    // Find the index of the least significant bit
    std::uint64_t least_significant_bit_index = 0;
    std::uint64_t least_significant_compare_mask = mask;

    // We shift bits until we reach a 1, incrementing the counter each time
    while ((least_significant_compare_mask & 1) == 0) {
        least_significant_compare_mask >>= 1;
        least_significant_bit_index++;
    }
    return (1 << least_significant_bit_index) - 1;
}

std::uint64_t LinearCongruentialGenerator::getMaximumValue(const std::uint64_t modulus, std::uint64_t mask) {
    return std::min(modulus, mask);
}

// Generate a random value anywhere in the range of the LCG
std::uint64_t LinearCongruentialGenerator::generateRandomValue() {
    // Compute the standard LCG formula for the next value
    m_current_value = (m_multiplier * m_current_value + m_increment) % m_modulus;

    // Apply the mask
    m_current_value = (m_current_value & m_mask);

    // Ensure the value is within the specified range
    m_current_value = m_current_value - m_minimum_value;

    return m_current_value;
}

// Generate a random value normalized to the range [0, 1)
double LinearCongruentialGenerator::generateUnitNormalRandomValue() {
  
    std::uint64_t random_value = generateRandomValue();

    std::uint64_t range = m_maximum_value - m_minimum_value;
    if (range == 0) {
        return 0.0; // Avoid division by zero
    }

    // Normalize the random value to [0, 1)
    double normalized_value = static_cast<double>(random_value - m_minimum_value) / range;
    return normalized_value;
}

