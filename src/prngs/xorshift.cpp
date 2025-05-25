#include <cstdint>
#include "xorshift.hpp"
#include "prng.hpp"


// Default constructor
XORShift::XORShift()
    : PseudoRandomNumberGenerator(),
      m_a(DefaultA),
      m_b(DefaultB),
      m_c(DefaultC),
      m_current_value(m_seed) {}

XORShift::XORShift(const uint64_t seed)
    : PseudoRandomNumberGenerator(seed),
      m_a(DefaultA),
      m_b(DefaultB),
      m_c(DefaultC),
      m_current_value(m_seed) {}

XORShift::XORShift(const uint64_t seed, const uint64_t a, const uint64_t b, const uint64_t c)
    : PseudoRandomNumberGenerator(seed),
      m_a(a),
      m_b(b),
      m_c(c),
      m_current_value(m_seed) {}



// Generate a random value anywhere in the range of the XORShift algorithm
uint64_t XORShift::generateRandomValue() {
    // Compute the standard LCG formula for the next value
    m_current_value ^= (m_current_value << m_a);
    m_current_value ^= (m_current_value >> m_b);
    m_current_value ^= (m_current_value << m_c);

    return m_current_value;
}

// Generate a random value normalized to the range [0, 1)
double XORShift::generateUnitNormalRandomValue() {
  
    uint64_t random_value = generateRandomValue();

    uint64_t range = m_maximum_value - m_minimum_value;
    if (range == 0) {
        return 0.0; // Avoid division by zero
    }

    // Normalize the random value to [0, 1)
    double normalized_value = static_cast<double>(random_value - m_minimum_value) / range;
    return normalized_value;
}

