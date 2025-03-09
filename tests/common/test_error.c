#include <gtest/gtest.h>
#include "retldb.h"

TEST(ErrorTest, ErrorStringValid) {
    EXPECT_STREQ("Success", retldb_error_string(RETLDB_OK));
    EXPECT_STREQ("Invalid argument", retldb_error_string(RETLDB_ERROR_INVALID_ARGUMENT));
    EXPECT_STREQ("Out of memory", retldb_error_string(RETLDB_ERROR_OUT_OF_MEMORY));
    EXPECT_STREQ("I/O error", retldb_error_string(RETLDB_ERROR_IO));
    EXPECT_STREQ("Corrupt data", retldb_error_string(RETLDB_ERROR_CORRUPT_DATA));
    EXPECT_STREQ("Item not found", retldb_error_string(RETLDB_ERROR_NOT_FOUND));
    EXPECT_STREQ("Item already exists", retldb_error_string(RETLDB_ERROR_ALREADY_EXISTS));
    EXPECT_STREQ("Operation not supported", retldb_error_string(RETLDB_ERROR_NOT_SUPPORTED));
    EXPECT_STREQ("Unknown error", retldb_error_string(RETLDB_ERROR_UNKNOWN));
}

TEST(ErrorTest, ErrorStringInvalid) {
    EXPECT_STREQ("Invalid error code", retldb_error_string(-1));
    EXPECT_STREQ("Invalid error code", retldb_error_string(RETLDB_ERROR_UNKNOWN + 1));
} 