#include <vector>
#include <cstdint>
#include <limits>
#include "prng.hpp"
#include "mersenne_twister.hpp"


MersenneTwister::MersenneTwister()
    : PseudoRandomNumberGenerator() {
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
    m_lower_bit_mask = std::numeric_limits<uint64_t>::max() >> (m_w - m_r);
    m_upper_bit_mask = std::numeric_limits<uint64_t>::max() << (m_r);
    m_recurrence_state = initializeRecurrenceState(m_seed);
    m_state_index = 0;
}

MersenneTwister::MersenneTwister(const uint64_t seed)
    : PseudoRandomNumberGenerator(seed) {
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
    m_lower_bit_mask = std::numeric_limits<uint64_t>::max() >> (m_w - m_r);
    m_upper_bit_mask = std::numeric_limits<uint64_t>::max() << (m_r);
    m_recurrence_state = initializeRecurrenceState(m_seed);
    m_state_index = 0;
}

std::vector<uint64_t> MersenneTwister::initializeRecurrenceState(const uint64_t seed) {
    std::vector<uint64_t> state(m_n);
    state[0] = seed;
    for (int i = 1; i < m_n; ++i) {
        state[i] = (m_f * (state[i - 1] ^ (state[i - 1] >> (m_w - 2)))) + i;
    }
    return state;
}

uint64_t MersenneTwister::generateNextStateValue(){

    int k = m_state_index;

    // x_{k-n}, but since we are using circular indexing it is actually just the same as x_k!
    int upper_index = k; 
    
     // x_{k-(n-1)}
    int lower_index = k - (m_n - 1);
    if (lower_index < 0){ // wraparound the index for the buffer
        lower_index += m_n;
    }

    uint64_t upper_part = m_recurrence_state[upper_index] & m_upper_bit_mask;
    uint64_t lower_part = m_recurrence_state[lower_index] & m_lower_bit_mask;
    uint64_t concatenated_value = upper_part | lower_part;

    uint64_t matrix_mul_result = concatenated_value >> 1;
    if (concatenated_value & 0b1){
        matrix_mul_result ^= m_a;
    }

    
    int middle_index = k - (m_n - m_m);
    if (middle_index < 0){
        middle_index += m_n;
    }
    uint64_t middle_value = m_recurrence_state[middle_index];

    uint64_t state_value = matrix_mul_result ^ middle_value; 
    
    m_recurrence_state[k] = state_value;

    m_state_index++;
    if (m_state_index >= m_n){
        m_state_index = 0;
    }

    return state_value;
    
}

uint64_t MersenneTwister::tempering(uint64_t val){
    uint64_t tempered_value = val ^ (val >> m_u);
    tempered_value ^= ((tempered_value << m_s) & m_b);
    tempered_value ^= ((tempered_value << m_t) & m_c);
    tempered_value ^= (tempered_value >> 1);
    return tempered_value;
}

uint64_t MersenneTwister::generateRandomValue(){
    uint64_t raw_value = generateNextStateValue();
    uint64_t tempered_value = tempering(raw_value);
    return tempered_value;
}

// Generate a random value normalized to the range [0, 1)
double MersenneTwister::generateUnitNormalRandomValue() {
  
    uint64_t random_value = generateRandomValue();

    uint64_t range = m_maximum_value - m_minimum_value;
    if (range == 0) {
        return 0.0; // Avoid division by zero
    }

    // Normalize the random value to [0, 1)
    double normalized_value = static_cast<double>(random_value - m_minimum_value) / static_cast<double>(range);
    return normalized_value;
}
