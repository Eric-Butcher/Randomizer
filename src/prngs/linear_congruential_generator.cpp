#include <cstdint>
#include "linear_congruential_generator.hpp"
#include "prng.hpp"


// Default constructor
LinearCongruentialGenerator::LinearCongruentialGenerator()
    : PseudoRandomNumberGenerator(getMinimumValueFromMask(DefaultMask), getMaximumValueFromMask(DefaultMask)),
      m_modulus(DefaultModulus),
      m_multiplier(DefaultMultiplier),
      m_increment(DefaultIncrement),
      m_mask(DefaultMask),
      m_current_value(m_seed) {}

// Constructor with seed
LinearCongruentialGenerator::LinearCongruentialGenerator(const std::uint64_t seed)
    : PseudoRandomNumberGenerator(seed, getMinimumValueFromMask(DefaultMask), getMaximumValueFromMask(DefaultMask)),
      m_modulus(DefaultModulus),
      m_multiplier(DefaultMultiplier),
      m_increment(DefaultIncrement),
      m_mask(DefaultMask),
      m_current_value(m_seed) {}

// Constructor with custom parameters
LinearCongruentialGenerator::LinearCongruentialGenerator(std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
    : PseudoRandomNumberGenerator(getMinimumValueFromMask(mask), getMaximumValueFromMask(mask)),
      m_modulus(modulus),
      m_multiplier(multiplier),
      m_increment(increment),
      m_mask(mask),
      m_current_value(m_seed) {}

// Constructor with seed and custom parameters
LinearCongruentialGenerator::LinearCongruentialGenerator(std::uint64_t seed, std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
    : PseudoRandomNumberGenerator(seed, getMinimumValueFromMask(mask), getMaximumValueFromMask(mask)),
      m_modulus(modulus),
      m_multiplier(multiplier),
      m_increment(increment),
      m_mask(mask),
      m_current_value(m_seed) {}


// The minimum and maximum values can be determined by taking into 
// account the bit indexes of the less significant and most significant
// bits of the mask. The minimum value would be 2^{i} - 1, where i is the
// index of the least significant bit of the mask. The maximum value
// would be 2^{j} - 1, where j is the index of the most significant bit
// of the mask.
static std::uint64_t getMinimumValueFromMask(const std::uint64_t mask) {

    // Find the index of the least significant bit
    std::uint64_t least_significant_bit_index = 0;
    std::uint64_t least_significant_compare_mask = mask;
    while ((least_significant_compare_mask & 1) == 0) {
        least_significant_compare_mask >>= 1;
        least_significant_bit_index++;
    }
    return (1 << least_significant_bit_index) - 1;
}

static std::uint64_t getMaximumValueFromMask(const std::uint64_t mask) {
    // Find the index of the most significant bit
    std::uint64_t most_significant_bit_index = 0;
    std::uint64_t most_significant_compare_mask = mask;
    while (most_significant_compare_mask > 1) {
        most_significant_compare_mask >>= 1;
        most_significant_bit_index++;
    }

    return (1 << most_significant_bit_index) - 1;
}

// Generate a random value anywhere in the range of the LCG
std::uint64_t LinearCongruentialGenerator::generateRandomValue() {
    // Compute the standard LCG formula for the next value
    m_current_value = (m_multiplier * m_current_value + m_increment) % m_modulus;

    // Apply the mask
    m_current_value = m_current_value & m_mask;

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

