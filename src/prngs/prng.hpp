#ifndef PRNG_H
#define PRNG_H

#include <cstdint>

class PseudoRandomNumberGenerator 
{
    private:
        std::uint64_t seed;

    public:
        PseudoRandomNumberGenerator();

        PseudoRandomNumberGenerator(const std::uint64_t seed);

        std::uint64_t generateCryptographicallyInsecureSeed();

        virtual uint64_t generateRandomValue() = 0;
};

#endif