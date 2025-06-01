#include "gtest/gtest.h"
#include "prng.hpp"
#include "xorshift.hpp"
#include <cstdint>
#include <limits>
#include <cmath>

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

    const int enough_iterations_to_be_confident = 1000;
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


TEST(TestXORShift, GenerateFloatingPointRandomValueInRange){
    XORShift xor_shift = XORShift();
    const float min = -25.678f;
    const float max = 3242.342f;
    const int num_iterations = 1000;
    for (int i = 0; i < num_iterations; ++i){
        double value = xor_shift.generateFloatingPointRandomValue(min, max);
        ASSERT_GE(value, min);
        ASSERT_LE(value, max);
    }
}

TEST(TestXORShift, GenerateFloatingPointRandomValueEquidistributed){
    XORShift xor_shift = XORShift();
    const double min = -300.0;
    const double max = 700.0;
    const int num_iterations = 10000;
    double sum = 0.0;
    double variance_sum = 0.0;
    double expected_value = (min + max) / 2.0;
    for (int i = 0; i < num_iterations; ++i){
        double value = xor_shift.generateFloatingPointRandomValue(min, max);
        sum += value;
        variance_sum += (value - expected_value) * (value - expected_value);
    }
    double average = sum / num_iterations;
    double variance = variance_sum / num_iterations;
    double standard_deviation = std::sqrt(variance);
    double three_sigma = 3 * standard_deviation;
    ASSERT_NEAR(expected_value, average, three_sigma);

}

TEST(TestXORShift, GenerateIntegerRandomValueInRange){
    XORShift xor_shift = XORShift();
    const int32_t min = -70;
    const int32_t max = 141;
    const int num_iterations = 1000;
    for (int i = 0; i < num_iterations; ++i){
        int64_t value = xor_shift.generateIntegerRandomValue(min, max);
        ASSERT_GE(value, min);
        ASSERT_LE(value, max);
    }
}

TEST(TestXORShift, GenerateIntegerRandomValueEquidistributed){
    XORShift xor_shift = XORShift();
    const int32_t min = 1;
    const int32_t max = 6;
    const int num_iterations = 10000;
    double sum = 0.0;
    double expected_value = (min + max) / 2.0;
    for (int i = 0; i < num_iterations; ++i){
        sum += static_cast<double>(xor_shift.generateIntegerRandomValue(min, max)); 
    }
    double average = sum / num_iterations;
    double margin_of_error = 0.02 * expected_value;
    ASSERT_NEAR(expected_value, average, margin_of_error);
}

TEST(TestXORShift, GenerateIntegerRandomValueProducesInclusiveRange){
    XORShift xor_shift = XORShift();
    const int32_t min = 1;
    const int32_t max = 6;
    const int max_iterations = 1000;
    bool found_min = false;
    bool found_max = false;

    for (int i = 0; i < max_iterations; ++i) {
        int64_t value = xor_shift.generateIntegerRandomValue(min, max);
        if (value == min) {
            found_min = true;
        }
        if (value == max) {
            found_max = true;
        }
        if (found_min && found_max) {
            break; // No need to continue if both values are found
        }
    }

    ASSERT_TRUE(found_min);
    ASSERT_TRUE(found_max);
}