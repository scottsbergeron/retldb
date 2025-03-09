/**
 * @file test_file.c
 * @brief Tests for file operations
 */

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare functions from file.c that we'll test
extern int file_init(void);
extern int file_create(const char* filename);
extern void* file_open(const char* filename, const char* mode);
extern int file_close(void* file);

// Test fixture
class FileTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Cleanup code
    }
};

// Test file initialization
TEST_F(FileTest, Initialize) {
    EXPECT_EQ(0, file_init());
}

// Test file creation
TEST_F(FileTest, CreateFile) {
    const char* test_filename = "test_file.dat";
    
    // Remove file if it exists
    remove(test_filename);
    
    // Test creating a file
    EXPECT_EQ(0, file_create(test_filename));
    
    // Verify file exists
    FILE* fp = fopen(test_filename, "rb");
    EXPECT_NE(nullptr, fp);
    if (fp) {
        fclose(fp);
    }
    
    // Cleanup
    remove(test_filename);
}

// Test file open and close
TEST_F(FileTest, OpenAndCloseFile) {
    const char* test_filename = "test_open_close.dat";
    
    // Create a test file
    FILE* fp = fopen(test_filename, "wb");
    ASSERT_NE(nullptr, fp);
    fclose(fp);
    
    // Test opening the file
    void* file = file_open(test_filename, "rb");
    EXPECT_NE(nullptr, file);
    
    // Test closing the file
    EXPECT_EQ(0, file_close(file));
    
    // Cleanup
    remove(test_filename);
} 