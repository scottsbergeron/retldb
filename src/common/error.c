#include "retldb.h"

/**
 * Error message strings corresponding to retldb_error_t values
 */
static const char* error_messages[] = {
    "Success",                      /* RETLDB_OK */
    "Invalid argument",             /* RETLDB_ERROR_INVALID_ARGUMENT */
    "Out of memory",                /* RETLDB_ERROR_OUT_OF_MEMORY */
    "I/O error",                    /* RETLDB_ERROR_IO */
    "Corrupt data",                 /* RETLDB_ERROR_CORRUPT_DATA */
    "Item not found",               /* RETLDB_ERROR_NOT_FOUND */
    "Item already exists",          /* RETLDB_ERROR_ALREADY_EXISTS */
    "Operation not supported",      /* RETLDB_ERROR_NOT_SUPPORTED */
    "Unknown error"                 /* RETLDB_ERROR_UNKNOWN */
};

const char* retldb_error_string(retldb_error_t error) {
    if (error < 0 || error > RETLDB_ERROR_UNKNOWN) {
        return "Invalid error code";
    }
    return error_messages[error];
} 