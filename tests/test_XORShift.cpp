#include "gtest/gtest.h"
#include "prng.hpp"
#include "xorshift.hpp"

// Test that the lcg constructors do not fail
TEST(TestXORShift, Constructor) {
    EXPECT_NO_THROW(XORShift());
}