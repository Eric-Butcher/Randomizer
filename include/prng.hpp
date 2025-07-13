#ifndef PRNG_H
#define PRNG_H

#include <cstdint>

class PseudoRandomNumberGenerator {
protected:
    const uint64_t m_seed;
    const uint64_t m_minimum_value;
    const uint64_t m_maximum_value;

public:
    PseudoRandomNumberGenerator();
    PseudoRandomNumberGenerator(const uint64_t seed);
    PseudoRandomNumberGenerator(const uint64_t minimum_value, const uint64_t maximum_value);
    PseudoRandomNumberGenerator(const uint64_t seed, const uint64_t minimum_value, const uint64_t maximum_value);
    virtual ~PseudoRandomNumberGenerator() { }

    static uint64_t generateCryptographicallyInsecureSeed();

    virtual uint64_t generateRandomValue() = 0;
    double generateUnitNormalRandomValue();
    double generateFloatingPointRandomValue(float min, float max);
    int64_t generateIntegerRandomValue(int32_t min, int32_t max);
};

#endif