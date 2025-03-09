/**
 * @file datatype.c
 * @brief Implementation of data types for rETL DB
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * @brief Data type enumeration
 */
typedef enum {
    RETLDB_TYPE_NULL = 0,
    RETLDB_TYPE_BOOLEAN,
    RETLDB_TYPE_INT8,
    RETLDB_TYPE_INT16,
    RETLDB_TYPE_INT32,
    RETLDB_TYPE_INT64,
    RETLDB_TYPE_UINT8,
    RETLDB_TYPE_UINT16,
    RETLDB_TYPE_UINT32,
    RETLDB_TYPE_UINT64,
    RETLDB_TYPE_FLOAT,
    RETLDB_TYPE_DOUBLE,
    RETLDB_TYPE_STRING,
    RETLDB_TYPE_BINARY,
    RETLDB_TYPE_TIMESTAMP,
    RETLDB_TYPE_ARRAY,
    RETLDB_TYPE_MAP,
    RETLDB_TYPE_STRUCT
} retldb_type_id;

/**
 * @brief Data type structure
 */
typedef struct {
    retldb_type_id id;
    const char* name;
    size_t size;
    int (*compare)(const void*, const void*);
    void* (*copy)(const void*);
    void (*free)(void*);
    void* (*serialize)(const void*, size_t*);
    void* (*deserialize)(const void*, size_t);
} retldb_datatype_t;

// Global type registry
#define MAX_TYPES 32
static retldb_datatype_t g_type_registry[MAX_TYPES];
static int g_type_count = 0;

/**
 * @brief Initialize the data type system
 * 
 * @return 0 on success, non-zero on failure
 */
int datatype_init(void) {
    // Reset type registry
    memset(g_type_registry, 0, sizeof(g_type_registry));
    g_type_count = 0;
    
    // Register built-in types
    // In a real implementation, this would register all the built-in types
    // with their comparison, copy, serialization, and deserialization functions
    
    return 0;
}

/**
 * @brief Register a new data type
 * 
 * @param id Type ID
 * @param name Type name
 * @param size Size of the type in bytes (0 for variable-size types)
 * @param compare Comparison function
 * @param copy Copy function
 * @param free Free function
 * @param serialize Serialization function
 * @param deserialize Deserialization function
 * @return 0 on success, non-zero on failure
 */
int datatype_register(retldb_type_id id, const char* name, size_t size,
                      int (*compare)(const void*, const void*),
                      void* (*copy)(const void*),
                      void (*free)(void*),
                      void* (*serialize)(const void*, size_t*),
                      void* (*deserialize)(const void*, size_t)) {
    if (g_type_count >= MAX_TYPES) {
        return -1; // Type registry full
    }
    
    if (id < 0 || id >= MAX_TYPES) {
        return -1; // Invalid type ID
    }
    
    if (!name) {
        return -1; // Invalid name
    }
    
    // Check if type already exists
    for (int i = 0; i < g_type_count; i++) {
        if (g_type_registry[i].id == id) {
            return -1; // Type already registered
        }
    }
    
    // Register the type
    g_type_registry[g_type_count].id = id;
    g_type_registry[g_type_count].name = name;
    g_type_registry[g_type_count].size = size;
    g_type_registry[g_type_count].compare = compare;
    g_type_registry[g_type_count].copy = copy;
    g_type_registry[g_type_count].free = free;
    g_type_registry[g_type_count].serialize = serialize;
    g_type_registry[g_type_count].deserialize = deserialize;
    
    g_type_count++;
    
    return 0;
}

/**
 * @brief Get a data type by ID
 * 
 * @param id Type ID
 * @return Data type, NULL if not found
 */
const retldb_datatype_t* datatype_get_by_id(retldb_type_id id) {
    for (int i = 0; i < g_type_count; i++) {
        if (g_type_registry[i].id == id) {
            return &g_type_registry[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Get a data type by name
 * 
 * @param name Type name
 * @return Data type, NULL if not found
 */
const retldb_datatype_t* datatype_get_by_name(const char* name) {
    if (!name) {
        return NULL;
    }
    
    for (int i = 0; i < g_type_count; i++) {
        if (strcmp(g_type_registry[i].name, name) == 0) {
            return &g_type_registry[i];
        }
    }
    
    return NULL;
}

/**
 * @brief Compare two values of the same type
 * 
 * @param type Type of the values
 * @param a First value
 * @param b Second value
 * @return Negative if a < b, 0 if a == b, positive if a > b
 */
int datatype_compare(const retldb_datatype_t* type, const void* a, const void* b) {
    if (!type || !a || !b) {
        return 0;
    }
    
    if (type->compare) {
        return type->compare(a, b);
    }
    
    return 0;
}

/**
 * @brief Copy a value of a given type
 * 
 * @param type Type of the value
 * @param value Value to copy
 * @return Copy of the value, NULL on failure
 */
void* datatype_copy(const retldb_datatype_t* type, const void* value) {
    if (!type || !value) {
        return NULL;
    }
    
    if (type->copy) {
        return type->copy(value);
    }
    
    return NULL;
}

/**
 * @brief Free a value of a given type
 * 
 * @param type Type of the value
 * @param value Value to free
 */
void datatype_free(const retldb_datatype_t* type, void* value) {
    if (!type || !value) {
        return;
    }
    
    if (type->free) {
        type->free(value);
    }
}

/**
 * @brief Serialize a value of a given type
 * 
 * @param type Type of the value
 * @param value Value to serialize
 * @param size Pointer to store the size of the serialized data
 * @return Serialized data, NULL on failure
 */
void* datatype_serialize(const retldb_datatype_t* type, const void* value, size_t* size) {
    if (!type || !value || !size) {
        return NULL;
    }
    
    if (type->serialize) {
        return type->serialize(value, size);
    }
    
    return NULL;
}

/**
 * @brief Deserialize a value of a given type
 * 
 * @param type Type of the value
 * @param data Serialized data
 * @param size Size of the serialized data
 * @return Deserialized value, NULL on failure
 */
void* datatype_deserialize(const retldb_datatype_t* type, const void* data, size_t size) {
    if (!type || !data) {
        return NULL;
    }
    
    if (type->deserialize) {
        return type->deserialize(data, size);
    }
    
    return NULL;
} 