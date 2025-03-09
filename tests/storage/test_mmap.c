/**
 * @file test_mmap.c
 * @brief Tests for memory-mapped file operations
 */

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare functions from mmap.c that we'll test
extern int mmap_init(void);
extern void* mmap_file(const char* filename, size_t size, int read_only);
extern void* mmap_get_addr(void* handle);
extern size_t mmap_get_size(void* handle);
extern int mmap_unmap(void* handle);

// Test fixture
class MmapTest : public ::testing::Test {
protected:
    const char* test_filename = "test_mmap.dat";
    const size_t test_file_size = 4096;
    
    void SetUp() override {
        // Create a test file with some data
        FILE* fp = fopen(test_filename, "wb");
        ASSERT_NE(nullptr, fp);
        
        // Fill with pattern data
        char buffer[256];
        for (int i = 0; i < 256; i++) {
            buffer[i] = (char)i;
        }
        
        for (size_t i = 0; i < test_file_size / 256; i++) {
            ASSERT_EQ(256, fwrite(buffer, 1, 256, fp));
        }
        
        fclose(fp);
    }

    void TearDown() override {
        // Remove test file
        remove(test_filename);
    }
};

// Test initialization
TEST_F(MmapTest, Initialize) {
    EXPECT_EQ(0, mmap_init());
}

// Test mapping a file read-only
TEST_F(MmapTest, MapFileReadOnly) {
    void* handle = mmap_file(test_filename, 0, 1);
    ASSERT_NE(nullptr, handle);
    
    void* addr = mmap_get_addr(handle);
    ASSERT_NE(nullptr, addr);
    
    size_t size = mmap_get_size(handle);
    EXPECT_EQ(test_file_size, size);
    
    // Verify the data
    unsigned char* data = (unsigned char*)addr;
    for (size_t i = 0; i < test_file_size; i++) {
        EXPECT_EQ((unsigned char)(i % 256), data[i]);
    }
    
    EXPECT_EQ(0, mmap_unmap(handle));
}

// Test mapping a file read-write
TEST_F(MmapTest, MapFileReadWrite) {
    void* handle = mmap_file(test_filename, 0, 0);
    ASSERT_NE(nullptr, handle);
    
    void* addr = mmap_get_addr(handle);
    ASSERT_NE(nullptr, addr);
    
    size_t size = mmap_get_size(handle);
    EXPECT_EQ(test_file_size, size);
    
    // Modify the data
    unsigned char* data = (unsigned char*)addr;
    for (size_t i = 0; i < test_file_size; i++) {
        data[i] = (unsigned char)(255 - (i % 256));
    }
    
    EXPECT_EQ(0, mmap_unmap(handle));
    
    // Verify the changes persisted
    handle = mmap_file(test_filename, 0, 1);
    ASSERT_NE(nullptr, handle);
    
    addr = mmap_get_addr(handle);
    ASSERT_NE(nullptr, addr);
    
    data = (unsigned char*)addr;
    for (size_t i = 0; i < test_file_size; i++) {
        EXPECT_EQ((unsigned char)(255 - (i % 256)), data[i]);
    }
    
    EXPECT_EQ(0, mmap_unmap(handle));
}

// Test error handling
TEST_F(MmapTest, ErrorHandling) {
    // Test with invalid filename
    EXPECT_EQ(nullptr, mmap_file(nullptr, 0, 1));
    EXPECT_EQ(nullptr, mmap_file("nonexistent_file.dat", 0, 1));
    
    // Test with invalid handle
    EXPECT_EQ(nullptr, mmap_get_addr(nullptr));
    EXPECT_EQ(0, mmap_get_size(nullptr));
    EXPECT_NE(0, mmap_unmap(nullptr));
} 