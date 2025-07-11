#ifndef MERSENNE_TWISTER_H
#define MERSENNE_TWISTER_H

#include <cstdint>
#include <vector>
#include "prng.hpp"

class MersenneTwister : public PseudoRandomNumberGenerator {

private:

    /* MT19937-64 coefficients:
    w = 64 , word size in bits
    n = 312, degree of recurrence
    m = 156, middle word
    r = 31, number of bits of the lower bit mask
    a = 0xb5026f5aa96619e9, coefficients
    u = 29, tempering bit shift/mask
    d = 0x5555555555555555
    s = 17, tempering bit shift/mask
    b = 0x71d67fffeda60000, tempering bit mask
    t = 37, tempering bit shift/mask
    c = 0xfff7eee000000000, tempering bit mask
    l = 43, tempering bit shift/mask
    f = 6364136223846793005
    taken from @misc{ enwiki:1290406357,
    author = "{Wikipedia contributors}",
    title = "Mersenne Twister --- {Wikipedia}{,} The Free Encyclopedia",
    year = "2025",
    url = "https://en.wikipedia.org/w/index.php?title=Mersenne_Twister&oldid=1290406357",
    note = "[Online; accessed 5-June-2025]"
    }
    */

    static constexpr int Default_w = 64;
    static constexpr int Default_n = 312;
    static constexpr int Default_m = 156;
    static constexpr int Default_r = 31;
    static constexpr uint64_t Default_a = 0xb5026f5aa96619e9;
    static constexpr uint64_t Default_u = 29;
    static constexpr uint64_t Default_d = 0x5555555555555555;
    static constexpr uint64_t Default_s = 17;
    static constexpr uint64_t Default_b = 0x71d67fffeda60000;
    static constexpr uint64_t Default_t = 37;
    static constexpr uint64_t Default_c = 0xfff7eee000000000;
    static constexpr uint64_t Default_l = 43;
    static constexpr uint64_t Default_f = 6364136223846793005;
    
    // word size
    int m_w;

    // degree of the recurrence
    int m_n;

    // middle word offset
    int m_m;

    // separation point where the upper bitmask begins
    int m_r;

    // twist matrix coefficient
    uint64_t m_a;

    // MT bitwise constant
    uint64_t m_u;

    // MT bitwise constant
    uint64_t m_d;

    // bit shift used for tempering
    uint64_t m_s;

    // bit mask used for tempering
    uint64_t m_b;

    // bit shift used for tempering
    uint64_t m_t;

    // bit mask used for tempering
    uint64_t m_c;

    // MT bitwise constant
    uint64_t m_l;

    // initialization constant
    uint64_t m_f;

    uint64_t m_upper_bit_mask;
    uint64_t m_lower_bit_mask;
    std::vector<uint64_t> m_recurrence_state;
    int m_state_index;
    

    std::vector<uint64_t> initializeRecurrenceState(const uint64_t seed);
    uint64_t generateNextStateValue();
    uint64_t tempering(uint64_t);

public:
    MersenneTwister();
    MersenneTwister(const uint64_t seed);

    uint64_t generateRandomValue() override;
};

#endif