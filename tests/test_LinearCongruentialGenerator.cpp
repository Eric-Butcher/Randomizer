#include "gtest/gtest.h"
#include "prng.hpp"
#include "linear_congruential_generator.hpp"
#include <cstdint>
#include <limits>

// Test that the lcg constructors do not fail
TEST(TestLinearCongruentialGenerator, BlankConstructor) {
    EXPECT_NO_THROW(LinearCongruentialGenerator());
}

TEST(TestLinearCongruentialGenerator, SeedConstructor) {
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::min()));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::min() + 1));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::max() / 10));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::max() / 2));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::max() - 1));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::max()));
}

TEST(TestLinearCongruentialGenerator, CustomConstructor) {
    EXPECT_NO_THROW(LinearCongruentialGenerator(0x7FFFFFFF, 1103515245, 12345, 0x7FFFFFFF));
    EXPECT_NO_THROW(LinearCongruentialGenerator(60, 3453983, 897, 0xFFFFFFFF));
    EXPECT_NO_THROW(LinearCongruentialGenerator(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));
    EXPECT_NO_THROW(LinearCongruentialGenerator(1, std::numeric_limits<uint64_t>::max() / 2, -50, 0xFFF));
}

TEST(TestLinearCongruentialGenerator, SeedAndCustomConstructor) {
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::min(), 0x7FFFFFFF, 1103515245, 12345, 0x7FFFFFFF));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::max(), 60, 3453983, 897, 0xFFFFFFFF));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::max() / 2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));
    EXPECT_NO_THROW(LinearCongruentialGenerator(std::numeric_limits<uint64_t>::max() - 1, 1, std::numeric_limits<uint64_t>::max() / 2, 58678575, 0xFFFFFF));
}
