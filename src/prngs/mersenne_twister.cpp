#include <vector>
#include <cstdint>
#include "prng.hpp"
#include "mersenne_twister.hpp"


MersenneTwister::MersenneTwister(){

}

MersenneTwister::MersenneTwister(const uint64_t seed) {
    m_w = Default_w;
    m_n = Default_n;
    m_m = Default_m;
    m_r = Default_r;
    m_a = Default_a;
    m_u = Default_u;
    m_d = Default_d;
    m_s = Default_s;
    m_b = Default_b;
    m_t = Default_t;
    m_c = Default_c;
    m_l = Default_l;
    m_f = Default_f;
    m_recurrence_state = initializeRecurrenceState(seed);
    m_current_value = seed;
}

std::vector<uint64_t> MersenneTwister::initializeRecurrenceState(const uint64_t seed) {
    std::vector<uint64_t> state(m_n);
    state[0] = seed;
    for (uint64_t i = 1; i < m_n; ++i) {
        state[i] = (m_f * (state[i - 1] ^ (state[i - 1] >> (m_w - 2)))) + i;
    }
    return state;
}


