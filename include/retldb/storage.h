/**
 * @file storage.h
 * @brief Storage module for rETL DB
 */

#ifndef RETLDB_STORAGE_H
#define RETLDB_STORAGE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize file operations
 * 
 * @return 0 on success, non-zero on failure
 */
int file_init(void);

/**
 * @brief Create a new file
 * 
 * @param filename The name of the file to create
 * @return 0 on success, non-zero on failure
 */
int file_create(const char* filename);

/**
 * @brief Open an existing file
 * 
 * @param filename The name of the file to open
 * @param mode The mode to open the file in
 * @return File handle on success, NULL on failure
 */
void* file_open(const char* filename, const char* mode);

/**
 * @brief Close a file
 * 
 * @param file The file handle to close
 * @return 0 on success, non-zero on failure
 */
int file_close(void* file);

/**
 * @brief Initialize memory mapping subsystem
 * 
 * @return 0 on success, non-zero on failure
 */
int mmap_init(void);

/**
 * @brief Map a file into memory
 * 
 * @param filename The name of the file to map
 * @param size The size of the mapping (0 for whole file)
 * @param read_only Whether the mapping should be read-only
 * @return Handle to the mapped file, NULL on failure
 */
void* mmap_file(const char* filename, size_t size, int read_only);

/**
 * @brief Get the address of the mapped memory
 * 
 * @param handle The memory-mapped file handle
 * @return Pointer to the mapped memory, NULL on failure
 */
void* mmap_get_addr(void* handle);

/**
 * @brief Get the size of the mapped memory
 * 
 * @param handle The memory-mapped file handle
 * @return Size of the mapped memory, 0 on failure
 */
size_t mmap_get_size(void* handle);

/**
 * @brief Unmap a memory-mapped file
 * 
 * @param handle The memory-mapped file handle
 * @return 0 on success, non-zero on failure
 */
int mmap_unmap(void* handle);

/**
 * @brief Initialize the buffer pool
 * 
 * @param capacity Maximum number of buffers in the pool
 * @param buffer_size Size of each buffer
 * @return 0 on success, non-zero on failure
 */
int buffer_init(size_t capacity, size_t buffer_size);

/**
 * @brief Clean up the buffer pool
 * 
 * @return 0 on success, non-zero on failure
 */
int buffer_cleanup(void);

/**
 * @brief Get a buffer from the pool
 * 
 * @param filename The filename associated with the buffer
 * @param offset The offset in the file
 * @return Buffer handle on success, NULL on failure
 */
void* buffer_get(const char* filename, size_t offset);

/**
 * @brief Mark a buffer as dirty
 * 
 * @param buffer The buffer handle
 * @return 0 on success, non-zero on failure
 */
int buffer_mark_dirty(void* buffer);

/**
 * @brief Flush a dirty buffer to disk
 * 
 * @param buffer The buffer handle
 * @return 0 on success, non-zero on failure
 */
int buffer_flush(void* buffer);

/**
 * @brief Flush all dirty buffers
 * 
 * @return 0 on success, non-zero on failure
 */
int buffer_flush_all(void);

#ifdef __cplusplus
}
#endif

#endif /* RETLDB_STORAGE_H */ 