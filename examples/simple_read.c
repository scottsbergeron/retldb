#include <stdio.h>
#include <stdlib.h>
#include "retldb.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <database_path>\n", argv[0]);
        return 1;
    }

    const char* db_path = argv[1];
    retldb_db_t* db = NULL;
    retldb_error_t err;

    // Open the database
    err = retldb_db_open(db_path, &db);
    if (err != RETLDB_OK) {
        printf("Failed to open database: %s\n", retldb_error_string(err));
        return 1;
    }

    printf("Successfully opened database at %s\n", db_path);

    // In a real application, we would perform read operations here

    // Close the database
    err = retldb_db_close(db);
    if (err != RETLDB_OK) {
        printf("Warning: Failed to close database: %s\n", retldb_error_string(err));
    }

    return 0;
} 