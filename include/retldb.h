/**
 * @file retldb.h
 * @brief Main header file for the rETL DB library
 *
 * This file contains the public API for the rETL DB database system,
 * a specialized database designed for Reverse ETL scenarios.
 */

#ifndef RETLDB_H
#define RETLDB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Library version information
 */
#define RETLDB_VERSION_MAJOR 0
#define RETLDB_VERSION_MINOR 1
#define RETLDB_VERSION_PATCH 0

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
 * @brief Get a string description of an error code
 *
 * @param error The error code
 * @return const char* String description of the error
 */
const char* retldb_error_string(retldb_error_t error);

/**
 * @brief Data types supported by rETL DB
 */
typedef enum {
    RETLDB_TYPE_NULL = 0,    /**< NULL value */
    RETLDB_TYPE_BOOLEAN,     /**< Boolean value */
    RETLDB_TYPE_INT8,        /**< 8-bit signed integer */
    RETLDB_TYPE_INT16,       /**< 16-bit signed integer */
    RETLDB_TYPE_INT32,       /**< 32-bit signed integer */
    RETLDB_TYPE_INT64,       /**< 64-bit signed integer */
    RETLDB_TYPE_UINT8,       /**< 8-bit unsigned integer */
    RETLDB_TYPE_UINT16,      /**< 16-bit unsigned integer */
    RETLDB_TYPE_UINT32,      /**< 32-bit unsigned integer */
    RETLDB_TYPE_UINT64,      /**< 64-bit unsigned integer */
    RETLDB_TYPE_FLOAT,       /**< 32-bit floating point */
    RETLDB_TYPE_DOUBLE,      /**< 64-bit floating point */
    RETLDB_TYPE_STRING,      /**< UTF-8 string */
    RETLDB_TYPE_BINARY,      /**< Binary data */
    RETLDB_TYPE_TIMESTAMP,   /**< Timestamp (64-bit) */
    RETLDB_TYPE_ARRAY,       /**< Array of values */
    RETLDB_TYPE_MAP,         /**< Key-value map */
    RETLDB_TYPE_STRUCT       /**< Structured data */
} retldb_type_t;

/**
 * @brief Database handle
 */
typedef struct retldb_db_t retldb_db_t;

/**
 * @brief Table handle
 */
typedef struct retldb_table_t retldb_table_t;

/**
 * @brief Schema handle
 */
typedef struct retldb_schema_t retldb_schema_t;

/**
 * @brief Column definition
 */
typedef struct {
    const char* name;     /**< Column name */
    retldb_type_t type;   /**< Column data type */
    int nullable;         /**< Whether the column can be NULL */
} retldb_column_def_t;

/**
 * @brief Create a new database
 *
 * @param path Path to the database directory
 * @param db Pointer to store the database handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_db_create(const char* path, retldb_db_t** db);

/**
 * @brief Open an existing database
 *
 * @param path Path to the database directory
 * @param db Pointer to store the database handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_db_open(const char* path, retldb_db_t** db);

/**
 * @brief Close a database
 *
 * @param db Database handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_db_close(retldb_db_t* db);

/**
 * @brief Create a new schema
 *
 * @param columns Array of column definitions
 * @param num_columns Number of columns
 * @param schema Pointer to store the schema handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_schema_create(
    const retldb_column_def_t* columns,
    size_t num_columns,
    retldb_schema_t** schema
);

/**
 * @brief Free a schema
 *
 * @param schema Schema handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_schema_free(retldb_schema_t* schema);

/**
 * @brief Create a new table
 *
 * @param db Database handle
 * @param name Table name
 * @param schema Schema handle
 * @param table Pointer to store the table handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_table_create(
    retldb_db_t* db,
    const char* name,
    retldb_schema_t* schema,
    retldb_table_t** table
);

/**
 * @brief Open an existing table
 *
 * @param db Database handle
 * @param name Table name
 * @param table Pointer to store the table handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_table_open(
    retldb_db_t* db,
    const char* name,
    retldb_table_t** table
);

/**
 * @brief Close a table
 *
 * @param table Table handle
 * @return retldb_error_t Error code
 */
retldb_error_t retldb_table_close(retldb_table_t* table);

/**
 * @brief Get the library version
 *
 * @param major Pointer to store the major version
 * @param minor Pointer to store the minor version
 * @param patch Pointer to store the patch version
 */
void retldb_version(int* major, int* minor, int* patch);

#ifdef __cplusplus
}
#endif

#endif /* RETLDB_H */ 