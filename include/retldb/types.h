/**
 * @file types.h
 * @brief Data types and schema definitions for rETL DB
 */

#ifndef RETLDB_TYPES_H
#define RETLDB_TYPES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

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
typedef struct retldb_datatype_t retldb_datatype_t;

/**
 * @brief Field structure
 */
typedef struct retldb_field_t retldb_field_t;

/**
 * @brief Schema structure
 */
typedef struct retldb_schema_t retldb_schema_t;

/**
 * @brief Initialize the data type system
 * 
 * @return 0 on success, non-zero on failure
 */
int datatype_init(void);

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
                      void* (*deserialize)(const void*, size_t));

/**
 * @brief Get a data type by ID
 * 
 * @param id Type ID
 * @return Data type, NULL if not found
 */
const retldb_datatype_t* datatype_get_by_id(retldb_type_id id);

/**
 * @brief Get a data type by name
 * 
 * @param name Type name
 * @return Data type, NULL if not found
 */
const retldb_datatype_t* datatype_get_by_name(const char* name);

/**
 * @brief Compare two values of the same type
 * 
 * @param type Type of the values
 * @param a First value
 * @param b Second value
 * @return Negative if a < b, 0 if a == b, positive if a > b
 */
int datatype_compare(const retldb_datatype_t* type, const void* a, const void* b);

/**
 * @brief Copy a value of a given type
 * 
 * @param type Type of the value
 * @param value Value to copy
 * @return Copy of the value, NULL on failure
 */
void* datatype_copy(const retldb_datatype_t* type, const void* value);

/**
 * @brief Free a value of a given type
 * 
 * @param type Type of the value
 * @param value Value to free
 */
void datatype_free(const retldb_datatype_t* type, void* value);

/**
 * @brief Serialize a value of a given type
 * 
 * @param type Type of the value
 * @param value Value to serialize
 * @param size Pointer to store the size of the serialized data
 * @return Serialized data, NULL on failure
 */
void* datatype_serialize(const retldb_datatype_t* type, const void* value, size_t* size);

/**
 * @brief Deserialize a value of a given type
 * 
 * @param type Type of the value
 * @param data Serialized data
 * @param size Size of the serialized data
 * @return Deserialized value, NULL on failure
 */
void* datatype_deserialize(const retldb_datatype_t* type, const void* data, size_t size);

/**
 * @brief Create a new schema
 * 
 * @param name Schema name
 * @return New schema, NULL on failure
 */
retldb_schema_t* schema_create(const char* name);

/**
 * @brief Free a schema
 * 
 * @param schema Schema to free
 */
void schema_free(retldb_schema_t* schema);

/**
 * @brief Add a field to a schema
 * 
 * @param schema Schema to add the field to
 * @param name Field name
 * @param type Field type
 * @param nullable Whether the field can be NULL
 * @param default_value Default value for the field
 * @return 0 on success, non-zero on failure
 */
int schema_add_field(retldb_schema_t* schema, const char* name,
                     const retldb_datatype_t* type, int nullable,
                     void* default_value);

/**
 * @brief Get a field from a schema by name
 * 
 * @param schema Schema to get the field from
 * @param name Field name
 * @return Field, NULL if not found
 */
const retldb_field_t* schema_get_field(const retldb_schema_t* schema, const char* name);

/**
 * @brief Get a field from a schema by index
 * 
 * @param schema Schema to get the field from
 * @param index Field index
 * @return Field, NULL if index is out of bounds
 */
const retldb_field_t* schema_get_field_by_index(const retldb_schema_t* schema, int index);

/**
 * @brief Get the number of fields in a schema
 * 
 * @param schema Schema to get the field count from
 * @return Number of fields, -1 on failure
 */
int schema_get_field_count(const retldb_schema_t* schema);

/**
 * @brief Serialize a schema to a binary format
 * 
 * @param schema Schema to serialize
 * @param size Pointer to store the size of the serialized data
 * @return Serialized data, NULL on failure
 */
void* schema_serialize(const retldb_schema_t* schema, size_t* size);

/**
 * @brief Deserialize a schema from a binary format
 * 
 * @param data Serialized data
 * @param size Size of the serialized data
 * @return Deserialized schema, NULL on failure
 */
retldb_schema_t* schema_deserialize(const void* data, size_t size);

/**
 * @brief Validate a schema
 * 
 * @param schema Schema to validate
 * @return 0 if valid, non-zero otherwise
 */
int schema_validate(const retldb_schema_t* schema);

#ifdef __cplusplus
}
#endif

#endif /* RETLDB_TYPES_H */
