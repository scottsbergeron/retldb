/**
 * @file error.h
 * @brief Error handling for rETL DB
 */

#ifndef RETLDB_ERROR_H
#define RETLDB_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Error codes returned by rETL DB functions
 */
typedef enum {
    RETLDB_OK = 0,                  /**< Operation successful */
    RETLDB_ERROR_INVALID_ARGUMENT,  /**< Invalid argument provided */
    RETLDB_ERROR_OUT_OF_MEMORY,     /**< Memory allocation failed */
    RETLDB_ERROR_IO,                /**< I/O error occurred */
    RETLDB_ERROR_CORRUPT_DATA,      /**< Data corruption detected */
    RETLDB_ERROR_NOT_FOUND,         /**< Requested item not found */
    RETLDB_ERROR_ALREADY_EXISTS,    /**< Item already exists */
    RETLDB_ERROR_NOT_SUPPORTED,     /**< Operation not supported */
    RETLDB_ERROR_UNKNOWN            /**< Unknown error */
} retldb_error_t;

/**
 * @brief Get a string description for an error code
 * 
 * @param error The error code
 * @return String description of the error
 */
const char* retldb_error_string(retldb_error_t error);

#ifdef __cplusplus
}
#endif

#endif /* RETLDB_ERROR_H */
