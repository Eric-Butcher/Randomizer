#ifndef LINEAR_CONGRUENTIAL_GENERATOR_H
#define LINEAR_CONGRUENTIAL_GENERATOR_H

#include <cstdint>
#include "prng.hpp"

class LinearCongruentialGenerator : public PseudoRandomNumberGenerator {
    // https://en.wikipedia.org/wiki/Linear_congruential_generator#

    /*
        LCGs are defined as X_{n+1} = (aX_{n} + c) \mod{m}
        Where X is the vector of pseudorandom values produced
        The values a, c, and m are constants
        m is the modulus
        a is the multiplier
        c is the increment

        glibc uses m=2^{31} a=1103515245 c=12345 and masks the result with 0x3FFFFFFF 
        which is what is used for the default implementation here
    */
private:
    
    // Defaults used from the glibc implementation, see:
    /* @misc{ enwiki:1280426923,
        author = "{Wikipedia contributors}",
        title = "Linear congruential generator --- {Wikipedia}{,} The Free Encyclopedia",
        year = "2025",
        url = "https://en.wikipedia.org/w/index.php?title=Linear_congruential_generator&oldid=1280426923",
        note = "[Online; accessed 10-May-2025]"
    } */

    static constexpr std::uint64_t DefaultModulus = 0x7FFFFFFF;      
    static constexpr std::uint64_t DefaultMultiplier = 1103515245;
    static constexpr std::uint64_t DefaultIncrement = 12345;
    static constexpr std::uint64_t DefaultMask = 0x7FFFFFFF;          // bits 0 through 30

    const std::uint64_t m_modulus;
    const std::uint64_t m_multiplier;
    const std::uint64_t m_increment;
    const std::uint64_t m_mask;
    std::uint64_t m_current_value = 0; 

public:
    LinearCongruentialGenerator();
    LinearCongruentialGenerator(const std::uint64_t seed);
    LinearCongruentialGenerator(std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask);
    LinearCongruentialGenerator(std::uint64_t seed, std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask);

    std::uint64_t generateRandomValue() override;
};

#endif