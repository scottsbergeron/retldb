/**
 * @file main.c
 * @brief Main entry point for rETL DB tests
 */

#include <gtest/gtest.h>

/**
 * @brief Main function for running all tests
 */
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 