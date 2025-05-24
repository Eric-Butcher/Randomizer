#include "gtest/gtest.h"
#include "prng.hpp"
#include "linear_congruential_generator.hpp"

// Test that the lcg constructors do not fail
TEST(TestLinearCongruentialGenerator, Constructor) {
    EXPECT_NO_THROW(LinearCongruentialGenerator());
}