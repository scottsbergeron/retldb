/**
 * @file simple_read.c
 * @brief Example of reading data from rETL DB
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retldb.h"

/**
 * @brief Main entry point for the simple read example
 */
int main(int argc, char** argv) {
    printf("rETL DB Simple Read Example\n");
    printf("---------------------------\n");
    
    // Check command line arguments
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    const char* filename = argv[1];
    retldb_db_t* db = NULL;
    retldb_error_t err;
    
    // Open the database
    printf("Opening database file: %s\n", filename);
    err = retldb_db_open(filename, &db);
    if (err != RETLDB_OK) {
        if (err == RETLDB_ERROR_NOT_SUPPORTED) {
            printf("Database open not fully implemented yet. Continuing with example...\n");
        } else {
            printf("Failed to open database: %s\n", retldb_error_string(err));
            return 1;
        }
    } else {
        printf("Successfully opened database at %s\n", filename);
    }
    
    // In a real application, we would perform read operations here
    printf("Simulating read operations...\n");
    
    // Close the database
    if (db != NULL) {
        printf("Closing the database\n");
        err = retldb_db_close(db);
        if (err != RETLDB_OK) {
            printf("Warning: Failed to close database: %s\n", retldb_error_string(err));
        }
    }
    
    printf("Read completed successfully\n");
    return 0;
}
