#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retldb/storage.h"

// Test fixture
class BufferTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize buffer pool with 10 buffers of 4KB each
        ASSERT_EQ(0, buffer_init(10, 4096));
    }

    void TearDown() override {
        // Clean up buffer pool
        buffer_cleanup();
    }
};

// Test initialization
TEST_F(BufferTest, Initialize) {
    // Cleanup from SetUp
    EXPECT_EQ(0, buffer_cleanup());
    
    // Test initialization with valid parameters
    EXPECT_EQ(0, buffer_init(10, 4096));
    
    // Test double initialization (should fail)
    EXPECT_NE(0, buffer_init(10, 4096));
    
    // Cleanup
    EXPECT_EQ(0, buffer_cleanup());
    
    // Test cleanup when not initialized
    EXPECT_NE(0, buffer_cleanup());
}

// Test buffer allocation
TEST_F(BufferTest, BufferAllocation) {
    // Get a buffer
    void* buffer1 = buffer_get("test.dat", 0);
    EXPECT_NE(nullptr, buffer1);
    
    // Get the same buffer again (should return the same pointer)
    void* buffer2 = buffer_get("test.dat", 0);
    EXPECT_EQ(buffer1, buffer2);
    
    // Get a different buffer
    void* buffer3 = buffer_get("test.dat", 4096);
    EXPECT_NE(nullptr, buffer3);
    EXPECT_NE(buffer1, buffer3);
    
    // Get a buffer for a different file
    void* buffer4 = buffer_get("other.dat", 0);
    EXPECT_NE(nullptr, buffer4);
    EXPECT_NE(buffer1, buffer4);
    EXPECT_NE(buffer3, buffer4);
}

// Test buffer eviction
TEST_F(BufferTest, BufferEviction) {
    // Fill the buffer pool
    void* buffers[15];
    for (int i = 0; i < 15; i++) {
        char filename[32];
        sprintf(filename, "test%d.dat", i);
        buffers[i] = buffer_get(filename, 0);
        EXPECT_NE(nullptr, buffers[i]);
    }
    
    // The first 5 buffers should have been evicted
    for (int i = 0; i < 5; i++) {
        char filename[32];
        sprintf(filename, "test%d.dat", i);
        void* buffer = buffer_get(filename, 0);
        EXPECT_NE(nullptr, buffer);
        EXPECT_NE(buffers[i], buffer);
    }
    
    // The last 10 buffers should still be in the pool
    for (int i = 5; i < 15; i++) {
        char filename[32];
        sprintf(filename, "test%d.dat", i);
        void* buffer = buffer_get(filename, 0);
        EXPECT_NE(nullptr, buffer);
        EXPECT_EQ(buffers[i], buffer);
    }
}

// Test dirty buffer management
TEST_F(BufferTest, DirtyBuffers) {
    // Get a buffer
    void* buffer = buffer_get("test.dat", 0);
    EXPECT_NE(nullptr, buffer);
    
    // Mark as dirty
    EXPECT_EQ(0, buffer_mark_dirty(buffer));
    
    // Flush the buffer
    EXPECT_EQ(0, buffer_flush(buffer));
    
    // Flush all buffers
    EXPECT_EQ(0, buffer_flush_all());
    
    // Test error handling
    EXPECT_NE(0, buffer_mark_dirty(nullptr));
    EXPECT_NE(0, buffer_flush(nullptr));
} 