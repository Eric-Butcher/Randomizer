#include "prng.hpp"
#include <cstdint>


class LinearCongruentialGenerator : PseudoRandomNumberGenerator {

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
    LinearCongruentialGenerator()
        : PseudoRandomNumberGenerator(),
          m_modulus(DefaultModulus),
          m_multiplier(DefaultMultiplier),
          m_increment(DefaultIncrement),
          m_mask(DefaultMask) {};

    LinearCongruentialGenerator(std::uint64_t seed)
        : PseudoRandomNumberGenerator(seed),
          m_modulus(DefaultModulus),
          m_multiplier(DefaultMultiplier),
          m_increment(DefaultIncrement),
          m_mask(DefaultMask) {};

    LinearCongruentialGenerator(std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
        : PseudoRandomNumberGenerator(),
          m_modulus(modulus),
          m_multiplier(multiplier),
          m_increment(increment),
          m_mask(mask) {};

    LinearCongruentialGenerator(std::uint64_t seed, std::uint64_t modulus, std::uint64_t multiplier, std::uint64_t increment, std::uint64_t mask)
        : PseudoRandomNumberGenerator(seed),
          m_modulus(modulus),
          m_multiplier(multiplier),
          m_increment(increment),
          m_mask(mask) {};



    private:

        // Defaults used from the glibc implementation, see:
        /* @misc{ enwiki:1280426923,
            author = "{Wikipedia contributors}",
            title = "Linear congruential generator --- {Wikipedia}{,} The Free Encyclopedia",
            year = "2025",
            url = "https://en.wikipedia.org/w/index.php?title=Linear_congruential_generator&oldid=1280426923",
            note = "[Online; accessed 10-May-2025]"
        } */
        static constexpr std::uint64_t DefaultModulus = 0x7FFFFFFF;       // 2^31 - 1
        static constexpr std::uint64_t DefaultMultiplier = 1103515245;
        static constexpr std::uint64_t DefaultIncrement = 12345;
        static constexpr std::uint64_t DefaultMask = 0x3FFFFFFF;

        const std::uint64_t m_modulus;
        const std::uint64_t m_multiplier;
        const std::uint64_t m_increment;
        const std::uint64_t m_mask;
        std::uint64_t m_current_value = 0; // Current value of the generator

    public:
        std::uint64_t generateRandomValue() override {

            // Compute the standard LCG formula for the next value
            m_current_value = (m_multiplier * m_current_value + m_increment) % m_modulus;

            // Apply the mask
            m_current_value = m_current_value & m_mask;

            return m_current_value;
        };

};