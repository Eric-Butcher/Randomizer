#ifndef XOR_SHIFT_H
#define XOR_SHIFT_H

#include <cstdint>
#include "prng.hpp"

class XORShift : public PseudoRandomNumberGenerator {

//     @misc{ enwiki:1287473197,
//     author = "{Wikipedia contributors}",
//     title = "Xorshift --- {Wikipedia}{,} The Free Encyclopedia",
//     year = "2025",
//     url = "https://en.wikipedia.org/w/index.php?title=Xorshift&oldid=1287473197",
//     note = "[Online; accessed 11-May-2025]"
//   }
private:
    

    // Default values from https://en.wikipedia.org/wiki/Xorshift
    static constexpr uint64_t DefaultA = 13;      
    static constexpr uint64_t DefaultB = 7;
    static constexpr uint64_t DefaultC = 17;

    // XORShift has 3 constants that are used for shifting
    const uint64_t m_a;
    const uint64_t m_b;
    const uint64_t m_c;
    uint64_t m_current_value = 0; 

public:
    XORShift();
    XORShift(const uint64_t seed);
    XORShift(const uint64_t seed, const uint64_t a, const uint64_t b, const uint64_t c);


    uint64_t generateRandomValue() override;
    double generateUnitNormalRandomValue() override;
};

#endif