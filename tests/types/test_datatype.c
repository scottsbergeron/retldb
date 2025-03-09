/**
 * @file test_datatype.c
 * @brief Tests for data type functionality
 */

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare functions from datatype.c that we'll test
extern int datatype_init(void);
extern int datatype_register(int id, const char* name, size_t size,
                            int (*compare)(const void*, const void*),
                            void* (*copy)(const void*),
                            void (*free)(void*),
                            void* (*serialize)(const void*, size_t*),
                            void* (*deserialize)(const void*, size_t));
extern const void* datatype_get_by_id(int id);
extern const void* datatype_get_by_name(const char* name);

// Simple comparison function for testing
static int test_compare(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

// Simple copy function for testing
static void* test_copy(const void* value) {
    int* copy = (int*)malloc(sizeof(int));
    if (copy) {
        *copy = *(int*)value;
    }
    return copy;
}

// Simple free function for testing
static void test_free(void* value) {
    free(value);
}

// Simple serialization function for testing
static void* test_serialize(const void* value, size_t* size) {
    *size = sizeof(int);
    int* data = (int*)malloc(*size);
    if (data) {
        *data = *(int*)value;
    }
    return data;
}

// Simple deserialization function for testing
static void* test_deserialize(const void* data, size_t size) {
    if (size != sizeof(int)) {
        return NULL;
    }
    
    int* value = (int*)malloc(sizeof(int));
    if (value) {
        *value = *(int*)data;
    }
    return value;
}

// Test fixture
class DataTypeTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize data type system
        ASSERT_EQ(0, datatype_init());
    }

    void TearDown() override {
        // Nothing to clean up
    }
};

// Test initialization
TEST_F(DataTypeTest, Initialize) {
    // Already initialized in SetUp
    // Test double initialization (should be idempotent)
    EXPECT_EQ(0, datatype_init());
}

// Test type registration
TEST_F(DataTypeTest, RegisterType) {
    // Register a test type
    EXPECT_EQ(0, datatype_register(100, "TEST_TYPE", sizeof(int),
                                  test_compare, test_copy, test_free,
                                  test_serialize, test_deserialize));
    
    // Try to register the same type ID again (should fail)
    EXPECT_NE(0, datatype_register(100, "ANOTHER_TYPE", sizeof(int),
                                  test_compare, test_copy, test_free,
                                  test_serialize, test_deserialize));
    
    // Register another type
    EXPECT_EQ(0, datatype_register(101, "ANOTHER_TYPE", sizeof(double),
                                  NULL, NULL, NULL, NULL, NULL));
}

// Test type lookup
TEST_F(DataTypeTest, LookupType) {
    // Register a test type
    EXPECT_EQ(0, datatype_register(100, "TEST_TYPE", sizeof(int),
                                  test_compare, test_copy, test_free,
                                  test_serialize, test_deserialize));
    
    // Look up by ID
    EXPECT_NE(nullptr, datatype_get_by_id(100));
    EXPECT_EQ(nullptr, datatype_get_by_id(999)); // Non-existent ID
    
    // Look up by name
    EXPECT_NE(nullptr, datatype_get_by_name("TEST_TYPE"));
    EXPECT_EQ(nullptr, datatype_get_by_name("NON_EXISTENT_TYPE"));
    EXPECT_EQ(nullptr, datatype_get_by_name(NULL));
} 