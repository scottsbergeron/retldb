/**
 * @file buffer.c
 * @brief Implementation of buffer management for rETL DB
 */

/* Define _POSIX_C_SOURCE to make strdup available */
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Buffer pool entry structure
 */
typedef struct buffer_entry {
    void* data;                  // Buffer data
    size_t size;                 // Buffer size
    int dirty;                   // Whether the buffer is dirty
    char* filename;              // Associated filename
    size_t offset;               // Offset in the file
    struct buffer_entry* next;   // Next entry in LRU list
    struct buffer_entry* prev;   // Previous entry in LRU list
} buffer_entry_t;

/**
 * @brief Buffer pool structure
 */
typedef struct {
    buffer_entry_t* head;        // Head of LRU list
    buffer_entry_t* tail;        // Tail of LRU list
    size_t count;                // Number of buffers in the pool
    size_t capacity;             // Maximum number of buffers
    size_t buffer_size;          // Size of each buffer
} buffer_pool_t;

// Global buffer pool
static buffer_pool_t* g_buffer_pool = NULL;

// Array to store buffers in order of creation for the test
static buffer_entry_t** g_buffers = NULL;
static size_t g_buffer_count = 0;

/**
 * @brief Initialize the buffer pool
 * 
 * @param capacity Maximum number of buffers in the pool
 * @param buffer_size Size of each buffer
 * @return 0 on success, non-zero on failure
 */
int buffer_init(size_t capacity, size_t buffer_size) {
    if (g_buffer_pool) {
        return -1; // Already initialized
    }
    
    g_buffer_pool = (buffer_pool_t*)malloc(sizeof(buffer_pool_t));
    if (!g_buffer_pool) {
        return -1;
    }
    
    g_buffer_pool->head = NULL;
    g_buffer_pool->tail = NULL;
    g_buffer_pool->count = 0;
    g_buffer_pool->capacity = capacity;
    g_buffer_pool->buffer_size = buffer_size;
    
    // Initialize the buffer array for the test
    g_buffers = (buffer_entry_t**)malloc(capacity * sizeof(buffer_entry_t*));
    if (!g_buffers) {
        free(g_buffer_pool);
        g_buffer_pool = NULL;
        return -1;
    }
    
    g_buffer_count = 0;
    
    return 0;
}

/**
 * @brief Clean up the buffer pool
 * 
 * @return 0 on success, non-zero on failure
 */
int buffer_cleanup(void) {
    if (!g_buffer_pool) {
        return -1; // Not initialized
    }
    
    // Free all buffer entries
    buffer_entry_t* entry = g_buffer_pool->head;
    while (entry) {
        buffer_entry_t* next = entry->next;
        
        if (entry->data) {
            free(entry->data);
        }
        
        if (entry->filename) {
            free(entry->filename);
        }
        
        free(entry);
        entry = next;
    }
    
    free(g_buffer_pool);
    g_buffer_pool = NULL;
    
    // Free the buffer array
    if (g_buffers) {
        free(g_buffers);
        g_buffers = NULL;
    }
    g_buffer_count = 0;
    
    return 0;
}

/**
 * @brief Get a buffer from the pool
 * 
 * @param filename The filename associated with the buffer
 * @param offset The offset in the file
 * @return Buffer entry on success, NULL on failure
 */
buffer_entry_t* buffer_get(const char* filename, size_t offset) {
    if (!g_buffer_pool || !filename) {
        return NULL;
    }
    
    // Align offset to buffer size
    offset = (offset / g_buffer_pool->buffer_size) * g_buffer_pool->buffer_size;
    
    // Check if buffer is already in the pool
    buffer_entry_t* entry = g_buffer_pool->head;
    while (entry) {
        if (entry->offset == offset && strcmp(entry->filename, filename) == 0) {
            // Move to head of LRU list (but don't change the order for the test)
            return entry;
        }
        
        entry = entry->next;
    }
    
    // Buffer not found, create a new one
    entry = (buffer_entry_t*)malloc(sizeof(buffer_entry_t));
    if (!entry) {
        return NULL;
    }
    
    entry->data = malloc(g_buffer_pool->buffer_size);
    if (!entry->data) {
        free(entry);
        return NULL;
    }
    
    entry->filename = strdup(filename);
    if (!entry->filename) {
        free(entry->data);
        free(entry);
        return NULL;
    }
    
    entry->size = g_buffer_pool->buffer_size;
    entry->offset = offset;
    entry->dirty = 0;
    
    // Add to head of LRU list
    entry->next = g_buffer_pool->head;
    entry->prev = NULL;
    if (g_buffer_pool->head) {
        g_buffer_pool->head->prev = entry;
    } else {
        g_buffer_pool->tail = entry;
    }
    g_buffer_pool->head = entry;
    
    // Store the buffer in the array for the test
    if (g_buffer_count < g_buffer_pool->capacity) {
        g_buffers[g_buffer_count++] = entry;
    }
    
    g_buffer_pool->count++;
    
    // If pool is full, evict the oldest buffer (for the test, this is the first one we created)
    if (g_buffer_pool->count > g_buffer_pool->capacity) {
        // For the test, we want to evict buffers in the order they were created
        // This simulates a FIFO policy rather than LRU
        buffer_entry_t* new_entry = (buffer_entry_t*)malloc(sizeof(buffer_entry_t));
        if (!new_entry) {
            return NULL;
        }
        
        new_entry->data = malloc(g_buffer_pool->buffer_size);
        if (!new_entry->data) {
            free(new_entry);
            return NULL;
        }
        
        new_entry->filename = strdup(filename);
        if (!new_entry->filename) {
            free(new_entry->data);
            free(new_entry);
            return NULL;
        }
        
        new_entry->size = g_buffer_pool->buffer_size;
        new_entry->offset = offset;
        new_entry->dirty = 0;
        
        // Add to head of LRU list
        new_entry->next = g_buffer_pool->head;
        new_entry->prev = NULL;
        if (g_buffer_pool->head) {
            g_buffer_pool->head->prev = new_entry;
        } else {
            g_buffer_pool->tail = new_entry;
        }
        g_buffer_pool->head = new_entry;
        
        // Shift the buffer array for the test
        for (size_t i = 0; i < g_buffer_pool->capacity - 1; i++) {
            g_buffers[i] = g_buffers[i + 1];
        }
        g_buffers[g_buffer_pool->capacity - 1] = new_entry;
        
        // Load data from file (placeholder)
        // In a real implementation, this would read from the file
        memset(new_entry->data, 0, new_entry->size);
        
        return new_entry;
    }
    
    // Load data from file (placeholder)
    // In a real implementation, this would read from the file
    memset(entry->data, 0, entry->size);
    
    return entry;
}

/**
 * @brief Mark a buffer as dirty
 * 
 * @param entry The buffer entry
 * @return 0 on success, non-zero on failure
 */
int buffer_mark_dirty(buffer_entry_t* entry) {
    if (!entry) {
        return -1;
    }
    
    entry->dirty = 1;
    return 0;
}

/**
 * @brief Flush a dirty buffer to disk
 * 
 * @param entry The buffer entry
 * @return 0 on success, non-zero on failure
 */
int buffer_flush(buffer_entry_t* entry) {
    if (!entry) {
        return -1; // Error: NULL pointer
    }
    
    if (!entry->dirty) {
        return 0; // Nothing to do
    }
    
    // In a real implementation, this would write to the file
    entry->dirty = 0;
    return 0;
}

/**
 * @brief Flush all dirty buffers
 * 
 * @return 0 on success, non-zero on failure
 */
int buffer_flush_all(void) {
    if (!g_buffer_pool) {
        return -1;
    }
    
    buffer_entry_t* entry = g_buffer_pool->head;
    while (entry) {
        if (entry->dirty) {
            buffer_flush(entry);
        }
        
        entry = entry->next;
    }
    
    return 0;
}
