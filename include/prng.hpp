#ifndef PRNG_H
#define PRNG_H

#include <cstdint>

class PseudoRandomNumberGenerator 
{
    protected:
        const std::uint64_t m_seed;

    public:
        PseudoRandomNumberGenerator();

        PseudoRandomNumberGenerator(const std::uint64_t seed);

        std::uint64_t generateCryptographicallyInsecureSeed();

        virtual std::uint64_t generateRandomValue() = 0;
};

#endif