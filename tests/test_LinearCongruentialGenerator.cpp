#include "gtest/gtest.h"
#include "prng.hpp"
#include "linear_congruential_generator.hpp"
#include <cstdint>
#include <limits>
#include <cmath>

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

    const int enough_iterations_to_be_confident = 10000;
    for (int i = 0; i < enough_iterations_to_be_confident; ++i) {
        const double value = lcg.generateUnitNormalRandomValue();
        EXPECT_GE(value, 0.0);
        EXPECT_LE(value, 1.0);
    }
}


/* This test of course is subject to some randomness 
it is possible to fail even if everything is working 
properly, but it would be incredibly unlikely to fail. 
If this test ever fails it should be investigated further 
manually. */ 
TEST(TestLinearCongruentialGenerator, GenerateUnitNormalRandomValueEquidistributed) {
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const double expected_average = 0.5;
    double sum = 0.0;
    const int enough_iterations_to_be_confident = 100000;
    for (int i = 0; i < enough_iterations_to_be_confident; ++i) {
        sum += lcg.generateUnitNormalRandomValue();
    }
    const double average = sum / enough_iterations_to_be_confident;
    const double confidence_interval = 0.01;
    EXPECT_NEAR(average, expected_average, confidence_interval);
}


TEST(TestLinearCongruentialGenerator, GenerateFloatingPointRandomValueInRange){
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const float min = -25.678f;
    const float max = 3242.342f;
    const int num_iterations = 1000;
    for (int i = 0; i < num_iterations; ++i){
        double value = lcg.generateFloatingPointRandomValue(min, max);
        ASSERT_GE(value, min);
        ASSERT_LE(value, max);
    }
}

TEST(TestLinearCongruentialGenerator, GenerateFloatingPointRandomValueEquidistributed){
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const double min = -300.0;
    const double max = 700.0;
    const int num_iterations = 10000;
    double sum = 0.0;
    double variance_sum = 0.0;
    double expected_value = (min + max) / 2.0;
    for (int i = 0; i < num_iterations; ++i){
        double value = lcg.generateFloatingPointRandomValue(min, max);
        sum += value;
        variance_sum += (value - expected_value) * (value - expected_value);
    }
    double average = sum / num_iterations;
    double variance = variance_sum / num_iterations;
    double standard_deviation = std::sqrt(variance);
    double three_sigma = 3 * standard_deviation;
    ASSERT_NEAR(expected_value, average, three_sigma);;

}

TEST(TestLinearCongruentialGenerator, GenerateIntegerRandomValueInRange){
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const int32_t min = -70;
    const int32_t max = 141;
    const int num_iterations = 1000;
    for (int i = 0; i < num_iterations; ++i){
        int64_t value = lcg.generateIntegerRandomValue(min, max);
        ASSERT_GE(value, min);
        ASSERT_LE(value, max);
    }
}

TEST(TestLinearCongruentialGenerator, GenerateIntegerRandomValueEquidistributed){
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const int32_t min = 1;
    const int32_t max = 6;
    const int num_iterations = 10000;
    double sum = 0;
    double expected_value = (min + max) / 2.0;
    for (int i = 0; i < num_iterations; ++i){
        sum += static_cast<double>(lcg.generateIntegerRandomValue(min, max)); 
    }
    double average = sum / num_iterations;
    double margin_of_error = 0.02 * expected_value;
    ASSERT_NEAR(expected_value, average, margin_of_error);
}


TEST(TestLinearCongruentialGenerator, GenerateIntegerRandomValueProducesInclusiveRangePositive){
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const int32_t min = 1;
    const int32_t max = 6;
    const int max_iterations = 1000;
    bool found_min = false;
    bool found_max = false;

    for (int i = 0; i < max_iterations; ++i) {
        int64_t value = lcg.generateIntegerRandomValue(min, max);
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

TEST(TestLinearCongruentialGenerator, GenerateIntegerRandomValueProducesInclusiveRangeNegative){
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const int32_t min = -6;
    const int32_t max = -1;
    const int max_iterations = 1000;
    bool found_min = false;
    bool found_max = false;

    for (int i = 0; i < max_iterations; ++i) {
        int64_t value = lcg.generateIntegerRandomValue(min, max);
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

TEST(TestLinearCongruentialGenerator, GenerateIntegerRandomValueProducesInclusiveRangeNegativeAndPositive){
    LinearCongruentialGenerator lcg = LinearCongruentialGenerator();
    const int32_t min = -10;
    const int32_t max = 10;
    const int max_iterations = 1000;
    bool found_min = false;
    bool found_max = false;

    for (int i = 0; i < max_iterations; ++i) {
        int64_t value = lcg.generateIntegerRandomValue(min, max);
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