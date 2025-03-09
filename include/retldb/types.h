/**
 * @file types.h
 * @brief Basic data type definitions for rETL DB
 *
 * This file defines the fundamental data types used throughout the database system.
 * It includes primitive types, their serialization/deserialization functions, and
 * type metadata structures.
 */

#ifndef RETLDB_TYPES_H
#define RETLDB_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration of all supported data types
 */
typedef enum {
    RETLDB_TYPE_NULL = 0,    /**< NULL value type */
    RETLDB_TYPE_BOOLEAN,     /**< Boolean type (true/false) */
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
    RETLDB_TYPE_TIMESTAMP,   /**< Timestamp (64-bit, microseconds since epoch) */
    RETLDB_TYPE_DATE,        /**< Date (32-bit, days since epoch) */
    RETLDB_TYPE_TIME,        /**< Time of day (microseconds since midnight) */
    RETLDB_TYPE_DECIMAL,     /**< Decimal type with configurable precision */
    RETLDB_TYPE_ARRAY,       /**< Array of values of the same type */
    RETLDB_TYPE_MAP,         /**< Key-value map */
    RETLDB_TYPE_STRUCT       /**< Structured type with named fields */
} retldb_type_id;

/**
 * @brief Flags for type modifiers
 */
typedef enum {
    RETLDB_TYPE_FLAG_NONE = 0,       /**< No flags */
    RETLDB_TYPE_FLAG_NULLABLE = 1,   /**< Type can be NULL */
    RETLDB_TYPE_FLAG_INDEXED = 2,    /**< Type is indexed */
    RETLDB_TYPE_FLAG_PRIMARY = 4     /**< Type is part of primary key */
} retldb_type_flags;

/**
 * @brief Decimal type configuration
 */
typedef struct {
    uint8_t precision;   /**< Total number of digits */
    uint8_t scale;       /**< Number of digits after decimal point */
} retldb_decimal_info;

// Forward declaration for complex types
struct retldb_type;
struct retldb_value;

/**
 * @brief Array type configuration
 */
typedef struct {
    struct retldb_type* element_type;  /**< Type of array elements */
} retldb_array_info;

/**
 * @brief Map type configuration
 */
typedef struct {
    struct retldb_type* key_type;    /**< Type of map keys */
    struct retldb_type* value_type;  /**< Type of map values */
} retldb_map_info;

/**
 * @brief Field definition for struct types
 */
typedef struct {
    char* name;                /**< Field name */
    struct retldb_type* type;  /**< Field type */
} retldb_field;

/**
 * @brief Struct type configuration
 */
typedef struct {
    uint32_t field_count;      /**< Number of fields */
    retldb_field* fields;      /**< Array of field definitions */
} retldb_struct_info;

/**
 * @brief Type metadata structure
 */
typedef struct retldb_type {
    retldb_type_id id;         /**< Type identifier */
    uint32_t flags;            /**< Type flags */
    
    /** Type-specific information */
    union {
        retldb_decimal_info decimal;  /**< Decimal type info */
        retldb_array_info array;      /**< Array type info */
        retldb_map_info map;          /**< Map type info */
        retldb_struct_info structure; /**< Struct type info */
    } info;
} retldb_type;

/**
 * @brief Map entry structure to avoid circular dependency
 */
typedef struct {
    void* key;   /**< Key value (pointer to retldb_value) */
    void* value; /**< Value (pointer to retldb_value) */
} retldb_map_entry;

/**
 * @brief Value container for any supported type
 */
typedef struct retldb_value {
    retldb_type type;          /**< Type metadata */
    bool is_null;              /**< Whether value is NULL */
    
    /** Value storage */
    union {
        bool boolean;          /**< Boolean value */
        int8_t int8;           /**< 8-bit signed integer value */
        int16_t int16;         /**< 16-bit signed integer value */
        int32_t int32;         /**< 32-bit signed integer value */
        int64_t int64;         /**< 64-bit signed integer value */
        uint8_t uint8;         /**< 8-bit unsigned integer value */
        uint16_t uint16;       /**< 16-bit unsigned integer value */
        uint32_t uint32;       /**< 32-bit unsigned integer value */
        uint64_t uint64;       /**< 64-bit unsigned integer value */
        float float32;         /**< 32-bit floating point value */
        double float64;        /**< 64-bit floating point value */
        struct {
            char* data;        /**< String data (null-terminated) */
            size_t length;     /**< String length (excluding null terminator) */
        } string;
        struct {
            uint8_t* data;     /**< Binary data */
            size_t length;     /**< Binary data length */
        } binary;
        int64_t timestamp;     /**< Timestamp value (microseconds since epoch) */
        int32_t date;          /**< Date value (days since epoch) */
        int64_t time;          /**< Time value (microseconds since midnight) */
        struct {
            int64_t value;     /**< Decimal value (scaled by 10^scale) */
            uint8_t precision; /**< Total number of digits */
            uint8_t scale;     /**< Number of digits after decimal point */
        } decimal;
        struct {
            struct retldb_value* elements; /**< Array elements */
            size_t length;                 /**< Number of elements */
            size_t capacity;               /**< Allocated capacity */
        } array;
        struct {
            retldb_map_entry* entries;     /**< Map entries */
            size_t length;                 /**< Number of entries */
            size_t capacity;               /**< Allocated capacity */
        } map;
        struct {
            struct retldb_value* fields;   /**< Field values */
            size_t field_count;            /**< Number of fields */
        } structure;
    } value;
} retldb_value;

/**
 * @brief Schema field definition
 */
typedef struct {
    char* name;                /**< Field name */
    retldb_type type;          /**< Field type */
    char* description;         /**< Field description (optional) */
} retldb_schema_field;

/**
 * @brief Schema definition
 */
typedef struct {
    char* name;                      /**< Schema name */
    uint32_t field_count;            /**< Number of fields */
    retldb_schema_field* fields;     /**< Array of field definitions */
    uint32_t primary_key_count;      /**< Number of primary key fields */
    uint32_t* primary_key_indices;   /**< Indices of primary key fields */
} retldb_schema;

/**
 * @brief Create a new type definition
 * 
 * @param type_id The type identifier
 * @param flags Type flags
 * @return A new type definition
 */
retldb_type retldb_type_create(retldb_type_id type_id, uint32_t flags);

/**
 * @brief Create a decimal type definition
 * 
 * @param precision Total number of digits
 * @param scale Number of digits after decimal point
 * @param flags Type flags
 * @return A new decimal type definition
 */
retldb_type retldb_type_create_decimal(uint8_t precision, uint8_t scale, uint32_t flags);

/**
 * @brief Create an array type definition
 * 
 * @param element_type Type of array elements
 * @param flags Type flags
 * @return A new array type definition
 */
retldb_type retldb_type_create_array(retldb_type element_type, uint32_t flags);

/**
 * @brief Create a map type definition
 * 
 * @param key_type Type of map keys
 * @param value_type Type of map values
 * @param flags Type flags
 * @return A new map type definition
 */
retldb_type retldb_type_create_map(retldb_type key_type, retldb_type value_type, uint32_t flags);

/**
 * @brief Create a struct type definition
 * 
 * @param fields Array of field definitions
 * @param field_count Number of fields
 * @param flags Type flags
 * @return A new struct type definition
 */
retldb_type retldb_type_create_struct(retldb_field* fields, uint32_t field_count, uint32_t flags);

/**
 * @brief Free resources associated with a type
 * 
 * @param type Type to free
 */
void retldb_type_free(retldb_type* type);

/**
 * @brief Create a new value
 * 
 * @param type Type of the value
 * @return A new value of the specified type
 */
retldb_value retldb_value_create(retldb_type type);

/**
 * @brief Create a NULL value
 * 
 * @param type Type of the NULL value
 * @return A new NULL value of the specified type
 */
retldb_value retldb_value_create_null(retldb_type type);

/**
 * @brief Free resources associated with a value
 * 
 * @param value Value to free
 */
void retldb_value_free(retldb_value* value);

/**
 * @brief Serialize a value to a binary representation
 * 
 * @param value Value to serialize
 * @param buffer Buffer to write to
 * @param buffer_size Size of the buffer
 * @return Number of bytes written, or -1 on error
 */
int64_t retldb_value_serialize(const retldb_value* value, uint8_t* buffer, size_t buffer_size);

/**
 * @brief Deserialize a value from a binary representation
 * 
 * @param buffer Buffer to read from
 * @param buffer_size Size of the buffer
 * @param type Expected type of the value
 * @param value Output value
 * @return Number of bytes read, or -1 on error
 */
int64_t retldb_value_deserialize(const uint8_t* buffer, size_t buffer_size, 
                                retldb_type type, retldb_value* value);

/**
 * @brief Create a new schema
 * 
 * @param name Schema name
 * @param fields Array of field definitions
 * @param field_count Number of fields
 * @param primary_key_indices Indices of primary key fields
 * @param primary_key_count Number of primary key fields
 * @return A new schema
 */
retldb_schema retldb_schema_create(const char* name, 
                                 retldb_schema_field* fields, 
                                 uint32_t field_count,
                                 uint32_t* primary_key_indices,
                                 uint32_t primary_key_count);

/**
 * @brief Free resources associated with a schema
 * 
 * @param schema Schema to free
 */
void retldb_schema_free(retldb_schema* schema);

/**
 * @brief Validate a value against a schema field
 * 
 * @param value Value to validate
 * @param field Schema field to validate against
 * @return true if valid, false otherwise
 */
bool retldb_schema_validate_field(const retldb_value* value, const retldb_schema_field* field);

/**
 * @brief Get the size of a type in bytes
 * 
 * @param type Type to get size of
 * @return Size in bytes, or 0 for variable-length types
 */
size_t retldb_type_get_size(const retldb_type* type);

/**
 * @brief Get a string representation of a type
 * 
 * @param type Type to get string representation of
 * @param buffer Buffer to write to
 * @param buffer_size Size of the buffer
 * @return Number of bytes written (excluding null terminator), or -1 on error
 */
int64_t retldb_type_to_string(const retldb_type* type, char* buffer, size_t buffer_size);

/**
 * @brief Get a string representation of a value
 * 
 * @param value Value to get string representation of
 * @param buffer Buffer to write to
 * @param buffer_size Size of the buffer
 * @return Number of bytes written (excluding null terminator), or -1 on error
 */
int64_t retldb_value_to_string(const retldb_value* value, char* buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* RETLDB_TYPES_H */
