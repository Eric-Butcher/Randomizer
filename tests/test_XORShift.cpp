#include "gtest/gtest.h"
#include "prng.hpp"
#include "xorshift.hpp"
#include <cstdint>
#include <limits>

// Test that the lcg constructors do not fail
TEST(TestXORShift, BlankConstructor) {
    EXPECT_NO_THROW(XORShift());
}

TEST(TestXORShift, SeedConstructor) {
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::min()));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::min() + 1));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::min() + 5));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max() / 5));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max() / 2));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max() - 1));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max()));
}


TEST(TestXORShift, SeedAndConstantsConstructor) {
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::min(), 1, 2, 3));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::min() + 1, 79, 55555, std::numeric_limits<uint64_t>::max()));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::min() + 5, 4545, 9876567, 9876786));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max() / 5, 1432, 6876, 3));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max() / 2, 42, 42, 42));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max() - 1, 2345678987, 9876543, 123422));
    EXPECT_NO_THROW(XORShift(std::numeric_limits<uint64_t>::max(), 345679875, 2, 0));
}


TEST(TestXORShift, GenerateUnitNormalRandomValueHasCorrectRange) {
    XORShift xor_shift = XORShift();

    const int enough_iterations_to_be_confident = 10000;
    for (int i = 0; i < enough_iterations_to_be_confident; ++i) {
        const double value = xor_shift.generateUnitNormalRandomValue();
        EXPECT_GE(value, 0.0);
        EXPECT_LE(value, 1.0);
    }
}


/* This test of course is subject to some randomness 
it is possible to fail even if everything is working 
properly, but it would be incredibly unlikely to fail. 
If this test ever fails it should be investigated further 
manually. */ 
TEST(TestXORShift, GenerateUnitNormalRandomValueEquidistributed) {
    XORShift xor_shift = XORShift();
    const double expected_average = 0.5;
    double sum = 0.0;
    const int enough_iterations_to_be_confident = 100000;
    for (int i = 0; i < enough_iterations_to_be_confident; ++i) {
        sum += xor_shift.generateUnitNormalRandomValue();
    }
    const double average = sum / enough_iterations_to_be_confident;
    const double confidence_interval = 0.01;
    EXPECT_NEAR(average, expected_average, confidence_interval);
}