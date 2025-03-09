/**
 * @file datatype.c
 * @brief Implementation of data types for rETL DB
 */

#include "retldb/types.h"
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

/**
 * @brief Create a new type definition
 */
retldb_type retldb_type_create(retldb_type_id type_id, uint32_t flags) {
    retldb_type type;
    type.id = type_id;
    type.flags = flags;
    
    // Initialize type-specific info to zeros
    memset(&type.info, 0, sizeof(type.info));
    
    return type;
}

/**
 * @brief Create a decimal type definition
 */
retldb_type retldb_type_create_decimal(uint8_t precision, uint8_t scale, uint32_t flags) {
    retldb_type type = retldb_type_create(RETLDB_TYPE_DECIMAL, flags);
    type.info.decimal.precision = precision;
    type.info.decimal.scale = scale;
    return type;
}

/**
 * @brief Create an array type definition
 * 
 * TODO: Implement deep copy of element_type
 */
retldb_type retldb_type_create_array(retldb_type element_type, uint32_t flags) {
    // TODO: Implement proper deep copy of element_type
    retldb_type type = retldb_type_create(RETLDB_TYPE_ARRAY, flags);
    
    // For now, just allocate memory for element_type and copy
    type.info.array.element_type = malloc(sizeof(retldb_type));
    if (type.info.array.element_type) {
        *type.info.array.element_type = element_type;
    }
    
    return type;
}

/**
 * @brief Create a map type definition
 * 
 * TODO: Implement deep copy of key_type and value_type
 */
retldb_type retldb_type_create_map(retldb_type key_type, retldb_type value_type, uint32_t flags) {
    // TODO: Implement proper deep copy of key_type and value_type
    retldb_type type = retldb_type_create(RETLDB_TYPE_MAP, flags);
    
    // For now, just allocate memory for key_type and value_type and copy
    type.info.map.key_type = malloc(sizeof(retldb_type));
    type.info.map.value_type = malloc(sizeof(retldb_type));
    
    if (type.info.map.key_type && type.info.map.value_type) {
        *type.info.map.key_type = key_type;
        *type.info.map.value_type = value_type;
    }
    
    return type;
}

/**
 * @brief Create a struct type definition
 * 
 * TODO: Implement deep copy of fields
 */
retldb_type retldb_type_create_struct(retldb_field* fields, uint32_t field_count, uint32_t flags) {
    // TODO: Implement proper deep copy of fields
    retldb_type type = retldb_type_create(RETLDB_TYPE_STRUCT, flags);
    
    type.info.structure.field_count = field_count;
    type.info.structure.fields = NULL;
    
    if (field_count > 0 && fields != NULL) {
        type.info.structure.fields = malloc(field_count * sizeof(retldb_field));
        
        if (type.info.structure.fields) {
            // For now, just do a shallow copy
            // TODO: Implement deep copy of field names and types
            memcpy(type.info.structure.fields, fields, field_count * sizeof(retldb_field));
        }
    }
    
    return type;
}

/**
 * @brief Free resources associated with a type
 * 
 * TODO: Implement proper cleanup for complex types
 */
void retldb_type_free(retldb_type* type) {
    if (!type) {
        return;
    }
    
    // Free resources based on type
    switch (type->id) {
        case RETLDB_TYPE_ARRAY:
            if (type->info.array.element_type) {
                retldb_type_free(type->info.array.element_type);
                free(type->info.array.element_type);
                type->info.array.element_type = NULL;
            }
            break;
            
        case RETLDB_TYPE_MAP:
            if (type->info.map.key_type) {
                retldb_type_free(type->info.map.key_type);
                free(type->info.map.key_type);
                type->info.map.key_type = NULL;
            }
            if (type->info.map.value_type) {
                retldb_type_free(type->info.map.value_type);
                free(type->info.map.value_type);
                type->info.map.value_type = NULL;
            }
            break;
            
        case RETLDB_TYPE_STRUCT:
            // TODO: Implement proper cleanup for struct fields
            if (type->info.structure.fields) {
                // For each field, free name and type
                for (uint32_t i = 0; i < type->info.structure.field_count; i++) {
                    if (type->info.structure.fields[i].name) {
                        free(type->info.structure.fields[i].name);
                    }
                    if (type->info.structure.fields[i].type) {
                        retldb_type_free(type->info.structure.fields[i].type);
                        free(type->info.structure.fields[i].type);
                    }
                }
                
                free(type->info.structure.fields);
                type->info.structure.fields = NULL;
            }
            break;
            
        default:
            // No resources to free for primitive types
            break;
    }
}

/**
 * @brief Create a new value
 * 
 * TODO: Implement proper initialization for complex types
 */
retldb_value retldb_value_create(retldb_type type) {
    retldb_value value;
    value.type = type;
    value.is_null = false;
    
    // Initialize value based on type
    memset(&value.value, 0, sizeof(value.value));
    
    // TODO: Implement proper initialization for complex types
    
    return value;
}

/**
 * @brief Create a NULL value
 */
retldb_value retldb_value_create_null(retldb_type type) {
    retldb_value value = retldb_value_create(type);
    value.is_null = true;
    return value;
}

/**
 * @brief Free resources associated with a value
 * 
 * TODO: Implement proper cleanup for complex types
 */
void retldb_value_free(retldb_value* value) {
    if (!value) {
        return;
    }
    
    // Free resources based on type
    if (!value->is_null) {
        switch (value->type.id) {
            case RETLDB_TYPE_STRING:
                if (value->value.string.data) {
                    free(value->value.string.data);
                    value->value.string.data = NULL;
                }
                break;
                
            case RETLDB_TYPE_BINARY:
                if (value->value.binary.data) {
                    free(value->value.binary.data);
                    value->value.binary.data = NULL;
                }
                break;
                
            case RETLDB_TYPE_ARRAY:
                // TODO: Implement proper cleanup for array elements
                if (value->value.array.elements) {
                    for (size_t i = 0; i < value->value.array.length; i++) {
                        retldb_value_free(&value->value.array.elements[i]);
                    }
                    free(value->value.array.elements);
                    value->value.array.elements = NULL;
                }
                break;
                
            case RETLDB_TYPE_MAP:
                // TODO: Implement proper cleanup for map entries
                if (value->value.map.entries) {
                    for (size_t i = 0; i < value->value.map.length; i++) {
                        retldb_value_free(&value->value.map.entries[i].key);
                        retldb_value_free(&value->value.map.entries[i].value);
                    }
                    free(value->value.map.entries);
                    value->value.map.entries = NULL;
                }
                break;
                
            case RETLDB_TYPE_STRUCT:
                // TODO: Implement proper cleanup for struct fields
                if (value->value.structure.fields) {
                    for (size_t i = 0; i < value->value.structure.field_count; i++) {
                        retldb_value_free(&value->value.structure.fields[i]);
                    }
                    free(value->value.structure.fields);
                    value->value.structure.fields = NULL;
                }
                break;
                
            default:
                // No resources to free for primitive types
                break;
        }
    }
}

/**
 * @brief Serialize a value to a binary representation
 * 
 * TODO: Implement serialization for all types
 */
int64_t retldb_value_serialize(const retldb_value* value, uint8_t* buffer, size_t buffer_size) {
    // TODO: Implement serialization for all types
    if (!value || !buffer || buffer_size == 0) {
        return -1;
    }
    
    // For now, just return an error
    return -1;
}

/**
 * @brief Deserialize a value from a binary representation
 * 
 * TODO: Implement deserialization for all types
 */
int64_t retldb_value_deserialize(const uint8_t* buffer, size_t buffer_size, 
                               retldb_type type, retldb_value* value) {
    // TODO: Implement deserialization for all types
    if (!buffer || buffer_size == 0 || !value) {
        return -1;
    }
    
    // For now, just return an error
    return -1;
}

/**
 * @brief Get the size of a type in bytes
 */
size_t retldb_type_get_size(const retldb_type* type) {
    if (!type) {
        return 0;
    }
    
    switch (type->id) {
        case RETLDB_TYPE_NULL:
            return 0;
        case RETLDB_TYPE_BOOLEAN:
            return sizeof(bool);
        case RETLDB_TYPE_INT8:
            return sizeof(int8_t);
        case RETLDB_TYPE_INT16:
            return sizeof(int16_t);
        case RETLDB_TYPE_INT32:
            return sizeof(int32_t);
        case RETLDB_TYPE_INT64:
            return sizeof(int64_t);
        case RETLDB_TYPE_UINT8:
            return sizeof(uint8_t);
        case RETLDB_TYPE_UINT16:
            return sizeof(uint16_t);
        case RETLDB_TYPE_UINT32:
            return sizeof(uint32_t);
        case RETLDB_TYPE_UINT64:
            return sizeof(uint64_t);
        case RETLDB_TYPE_FLOAT:
            return sizeof(float);
        case RETLDB_TYPE_DOUBLE:
            return sizeof(double);
        case RETLDB_TYPE_TIMESTAMP:
            return sizeof(int64_t);
        case RETLDB_TYPE_DATE:
            return sizeof(int32_t);
        case RETLDB_TYPE_TIME:
            return sizeof(int64_t);
        case RETLDB_TYPE_DECIMAL:
            return sizeof(int64_t) + 2 * sizeof(uint8_t); // value + precision + scale
        case RETLDB_TYPE_STRING:
        case RETLDB_TYPE_BINARY:
        case RETLDB_TYPE_ARRAY:
        case RETLDB_TYPE_MAP:
        case RETLDB_TYPE_STRUCT:
            // Variable-length types
            return 0;
        default:
            return 0;
    }
}

/**
 * @brief Get a string representation of a type
 * 
 * TODO: Implement string representation for complex types
 */
int64_t retldb_type_to_string(const retldb_type* type, char* buffer, size_t buffer_size) {
    if (!type || !buffer || buffer_size == 0) {
        return -1;
    }
    
    const char* type_name = "UNKNOWN";
    
    switch (type->id) {
        case RETLDB_TYPE_NULL:
            type_name = "NULL";
            break;
        case RETLDB_TYPE_BOOLEAN:
            type_name = "BOOLEAN";
            break;
        case RETLDB_TYPE_INT8:
            type_name = "INT8";
            break;
        case RETLDB_TYPE_INT16:
            type_name = "INT16";
            break;
        case RETLDB_TYPE_INT32:
            type_name = "INT32";
            break;
        case RETLDB_TYPE_INT64:
            type_name = "INT64";
            break;
        case RETLDB_TYPE_UINT8:
            type_name = "UINT8";
            break;
        case RETLDB_TYPE_UINT16:
            type_name = "UINT16";
            break;
        case RETLDB_TYPE_UINT32:
            type_name = "UINT32";
            break;
        case RETLDB_TYPE_UINT64:
            type_name = "UINT64";
            break;
        case RETLDB_TYPE_FLOAT:
            type_name = "FLOAT";
            break;
        case RETLDB_TYPE_DOUBLE:
            type_name = "DOUBLE";
            break;
        case RETLDB_TYPE_STRING:
            type_name = "STRING";
            break;
        case RETLDB_TYPE_BINARY:
            type_name = "BINARY";
            break;
        case RETLDB_TYPE_TIMESTAMP:
            type_name = "TIMESTAMP";
            break;
        case RETLDB_TYPE_DATE:
            type_name = "DATE";
            break;
        case RETLDB_TYPE_TIME:
            type_name = "TIME";
            break;
        case RETLDB_TYPE_DECIMAL:
            // TODO: Include precision and scale
            type_name = "DECIMAL";
            break;
        case RETLDB_TYPE_ARRAY:
            // TODO: Include element type
            type_name = "ARRAY";
            break;
        case RETLDB_TYPE_MAP:
            // TODO: Include key and value types
            type_name = "MAP";
            break;
        case RETLDB_TYPE_STRUCT:
            // TODO: Include field definitions
            type_name = "STRUCT";
            break;
    }
    
    int result = snprintf(buffer, buffer_size, "%s", type_name);
    
    if (result < 0 || (size_t)result >= buffer_size) {
        return -1;
    }
    
    return result;
}

/**
 * @brief Get a string representation of a value
 * 
 * TODO: Implement string representation for all types
 */
int64_t retldb_value_to_string(const retldb_value* value, char* buffer, size_t buffer_size) {
    if (!value || !buffer || buffer_size == 0) {
        return -1;
    }
    
    if (value->is_null) {
        int result = snprintf(buffer, buffer_size, "NULL");
        
        if (result < 0 || (size_t)result >= buffer_size) {
            return -1;
        }
        
        return result;
    }
    
    // TODO: Implement string representation for all types
    
    // For now, just return the type name
    char type_str[64];
    int64_t type_str_len = retldb_type_to_string(&value->type, type_str, sizeof(type_str));
    
    if (type_str_len < 0) {
        return -1;
    }
    
    int result = snprintf(buffer, buffer_size, "%s", type_str);
    
    if (result < 0 || (size_t)result >= buffer_size) {
        return -1;
    }
    
    return result;
}
