#include "gtest/gtest.h"
#include "prng.hpp"
#include "linear_congruential_generator.hpp"
#include "xorshift.hpp"

TEST(ArithmeticTest, OnePlusOneEqualsTwo) {
    ASSERT_EQ(1+1, 2) << "1+1 is 2";
}