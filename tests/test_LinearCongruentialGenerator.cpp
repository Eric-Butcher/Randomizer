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

TEST(TestLinearCongruentialGenerator, GenerateUnitNormalRandomValueHasCorrectRange) {
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();

    short enough_iterations_to_be_confident = 1000;
    for (short i = 0; i < enough_iterations_to_be_confident; ++i) {
        double value = lcg.generateUnitNormalRandomValue();
        EXPECT_GE(value, 0.0);
        EXPECT_LE(value, 1.0);
    }
}


/* This test of course is subject to some randomness 
it is possible to fail even if everything is working 
properly, but it would be incredibly unlikely to fail. 
If this test ever fails it should be investigated further 
manually. */ 
TEST(TestLinearCongruentialGenerator, GenerateUnitNormalRandomValueAverageConvergesToExpected) {
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    double expected_average = 0.5;
    double sum = 0.0;
    short enough_iterations_to_be_confident = 10000;
    for (short i = 0; i < enough_iterations_to_be_confident; ++i) {
        sum += lcg.generateUnitNormalRandomValue();
    }
    double average = sum / enough_iterations_to_be_confident;
    double confidence_interval = 1 / (enough_iterations_to_be_confident / 10);
    EXPECT_NEAR(average, expected_average, confidence_interval);
}
