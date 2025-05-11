#include <cstdint>
#include "linear_congruential_generator.hpp"
#include "prng.hpp"


// Default constructor
LinearCongruentialGenerator::LinearCongruentialGenerator()
    : PseudoRandomNumberGenerator(),
      m_modulus(DefaultModulus),
      m_multiplier(DefaultMultiplier),
      m_increment(DefaultIncrement),
      m_mask(DefaultMask),
      m_current_value(m_seed) {}

// Constructor with seed
LinearCongruentialGenerator::LinearCongruentialGenerator(const std::uint64_t seed)
    : PseudoRandomNumberGenerator(seed),
      m_modulus(DefaultModulus),
      m_multiplier(DefaultMultiplier),
      m_increment(DefaultIncrement),
      m_mask(DefaultMask),
      m_current_value(m_seed) {}

// Constructor with custom parameters
LinearCongruentialGenerator::LinearCongruentialGenerator(std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
    : PseudoRandomNumberGenerator(),
      m_modulus(modulus),
      m_multiplier(multiplier),
      m_increment(increment),
      m_mask(mask),
      m_current_value(m_seed) {}

// Constructor with seed and custom parameters
LinearCongruentialGenerator::LinearCongruentialGenerator(std::uint64_t seed, std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
    : PseudoRandomNumberGenerator(seed),
      m_modulus(modulus),
      m_multiplier(multiplier),
      m_increment(increment),
      m_mask(mask),
      m_current_value(m_seed) {}

// Generate a random value
std::uint64_t LinearCongruentialGenerator::generateRandomValue() {
    // Compute the standard LCG formula for the next value
    m_current_value = (m_multiplier * m_current_value + m_increment) % m_modulus;

    // Apply the mask
    m_current_value = m_current_value & m_mask;

    return m_current_value;
}