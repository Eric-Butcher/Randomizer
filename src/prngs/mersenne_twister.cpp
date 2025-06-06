#include <vector>
#include <cstdint>
#include "prng.hpp"
#include "mersenne_twister.hpp"


MersenneTwister::MersenneTwister(){

}

MersenneTwister::MersenneTwister(const uint64_t seed) {
    m_recurrence_state = initializeRecurrenceState(seed);
    m_current_value = m_recurrence_state[0];
}

std::vector<uint64_t> MersenneTwister::initializeRecurrenceState(const uint64_t seed, const uint64_t n, const uint64_t f, const uint64_t w) {
    std::vector<uint64_t> state(n);
    state[0] = seed;
    for (uint64_t i = 1; i < n; ++i) {
        state[i] = (f * (state[i - 1] ^ (state[i - 1] >> (w - 2)))) + i;
    }
    return state;
}


