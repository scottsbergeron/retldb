/**
 * @file retldb.h
 * @brief Main header file for rETL DB
 *
 * This file includes all the necessary headers for using rETL DB.
 */

#ifndef RETLDB_H
#define RETLDB_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "retldb/types.h"
#include "retldb/error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Database handle
 */
typedef struct retldb_db retldb_db_t;

/**
 * @brief Table handle
 */
typedef struct retldb_table retldb_table_t;

/**
 * @brief Column definition
 */
typedef struct {
    char* name;         /**< Column name */
    retldb_type type;   /**< Column data type */
    bool nullable;      /**< Whether column can be NULL */
    bool primary_key;   /**< Whether column is part of primary key */
} retldb_column_def_t;

/**
 * @brief Open a database
 *
 * @param path Path to database directory
 * @param create_if_missing Create database if it doesn't exist
 * @param db Output parameter for database handle
 * @return Error code
 */
retldb_error_t retldb_open(
    const char* path,
    bool create_if_missing,
    retldb_db_t** db
);

/**
 * @brief Close a database
 *
 * @param db Database handle
 * @return Error code
 */
retldb_error_t retldb_close(retldb_db_t* db);

/**
 * @brief Create a table schema
 *
 * @param name Table name
 * @param columns Column definitions
 * @param num_columns Number of columns
 * @param schema Output parameter for schema
 * @return Error code
 */
retldb_error_t retldb_create_schema(
    const char* name,
    retldb_column_def_t* columns,
    uint32_t num_columns,
    retldb_schema** schema
);

/**
 * @brief Free a schema
 *
 * @param schema Schema to free
 * @return Error code
 */
retldb_error_t retldb_free_schema(retldb_schema* schema);

/**
 * @brief Create a table
 *
 * @param db Database handle
 * @param schema Table schema
 * @param table Output parameter for table handle
 * @return Error code
 */
retldb_error_t retldb_create_table(
    retldb_db_t* db,
    retldb_schema* schema,
    retldb_table_t** table
);

/**
 * @brief Open a table
 *
 * @param db Database handle
 * @param name Table name
 * @param table Output parameter for table handle
 * @return Error code
 */
retldb_error_t retldb_open_table(
    retldb_db_t* db,
    const char* name,
    retldb_table_t** table
);

/**
 * @brief Close a table
 *
 * @param table Table handle
 * @return Error code
 */
retldb_error_t retldb_close_table(retldb_table_t* table);

/**
 * @brief Drop a table
 *
 * @param db Database handle
 * @param name Table name
 * @return Error code
 */
retldb_error_t retldb_drop_table(
    retldb_db_t* db,
    const char* name
);

#ifdef __cplusplus
}
#endif

#endif /* RETLDB_H */
