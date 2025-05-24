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

    short enough_iterations_to_be_confident = 1000;
    for (short i = 0; i < enough_iterations_to_be_confident; ++i) {
        double value = xor_shift.generateUnitNormalRandomValue();
        EXPECT_GE(value, 0.0);
        EXPECT_LE(value, 1.0);
    }
}


/* This test of course is subject to some randomness 
it is possible to fail even if everything is working 
properly, but it would be incredibly unlikely to fail. 
If this test ever fails it should be investigated further 
manually. */ 
TEST(TestXORShift, GenerateUnitNormalRandomValueAverageConvergesToExpected) {
    XORShift xor_shift = XORShift();
    double expected_average = 0.5;
    double sum = 0.0;
    short enough_iterations_to_be_confident = 10000;
    for (short i = 0; i < enough_iterations_to_be_confident; ++i) {
        sum += xor_shift.generateUnitNormalRandomValue();
    }
    double average = sum / enough_iterations_to_be_confident;
    double confidence_interval = 1 / (enough_iterations_to_be_confident / 10);
    EXPECT_NEAR(average, expected_average, confidence_interval);
}