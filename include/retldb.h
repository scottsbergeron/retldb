/**
 * @file retldb.h
 * @brief Main header file for the rETL DB library
 *
 * This file contains the public API for the rETL DB database system,
 * a specialized database designed for Reverse ETL scenarios.
 */

#ifndef RETLDB_H
#define RETLDB_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Library version information
 */
#define RETLDB_VERSION_MAJOR 0
#define RETLDB_VERSION_MINOR 1
#define RETLDB_VERSION_PATCH 0

/* Include module-specific headers */
#include "retldb/error.h"
#include "retldb/types.h"
#include "retldb/storage.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Database handle
 */
typedef struct retldb_db_t retldb_db_t;

/**
 * @brief Table handle
 */
typedef struct retldb_table_t retldb_table_t;

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
