#ifndef PRNG_H
#define PRNG_H

#include <cstdint>

class PseudoRandomNumberGenerator 
{
    protected:
        const uint64_t m_seed;
        const uint64_t m_minimum_value;
        const uint64_t m_maximum_value;


    public:
        PseudoRandomNumberGenerator();
        PseudoRandomNumberGenerator(const uint64_t seed);
        PseudoRandomNumberGenerator(const uint64_t minimum_value, const uint64_t maximum_value);
        PseudoRandomNumberGenerator(const uint64_t seed, const uint64_t minimum_value, const uint64_t maximum_value);

        uint64_t generateCryptographicallyInsecureSeed();

        virtual uint64_t generateRandomValue() = 0;
        virtual double generateUnitNormalRandomValue() = 0;
        virtual double generateFloatingPointRandomValue(double min, double max) = 0;
        virtual int64_t generateIntegerRandomValue(int64_t min, int64_t max) = 0;
};

#endif