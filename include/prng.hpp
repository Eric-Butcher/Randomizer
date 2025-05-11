#ifndef PRNG_H
#define PRNG_H

#include <cstdint>

class PseudoRandomNumberGenerator 
{
    protected:
        const std::uint64_t m_seed;
        const std::uint64_t m_minimum_value;
        const std::uint64_t m_maximum_value;


    public:
        PseudoRandomNumberGenerator();
        PseudoRandomNumberGenerator(const std::uint64_t minimum_value, const std::uint64_t maximum_value);
        PseudoRandomNumberGenerator(const std::uint64_t seed, const std::uint64_t minimum_value, const std::uint64_t maximum_value);

        std::uint64_t generateCryptographicallyInsecureSeed();

        virtual std::uint64_t generateRandomValue() = 0;
        virtual double generateUnitNormalRandomValue() = 0;
};

#endif