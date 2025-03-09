#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retldb/types.h"

// Test fixture
class SchemaTest : public ::testing::Test {
protected:
    retldb_schema_t* schema;
    const retldb_datatype_t* int_type;
    const retldb_datatype_t* string_type;
    
    void SetUp() override {
        // Initialize data type system
        ASSERT_EQ(0, datatype_init());
        
        // Register some test types
        ASSERT_EQ(0, datatype_register(RETLDB_TYPE_INT32, "INT", sizeof(int), NULL, NULL, NULL, NULL, NULL));
        ASSERT_EQ(0, datatype_register(RETLDB_TYPE_STRING, "STRING", 0, NULL, NULL, NULL, NULL, NULL));
        
        // Get the registered types
        int_type = datatype_get_by_id(RETLDB_TYPE_INT32);
        string_type = datatype_get_by_id(RETLDB_TYPE_STRING);
        
        ASSERT_NE(nullptr, int_type);
        ASSERT_NE(nullptr, string_type);
        
        // Create a test schema
        schema = schema_create("TEST_SCHEMA");
        ASSERT_NE(nullptr, schema);
    }

    void TearDown() override {
        // Free the schema
        schema_free(schema);
    }
};

// Test schema creation
TEST_F(SchemaTest, CreateSchema) {
    // Already created in SetUp
    
    // Test with NULL name (should fail)
    EXPECT_EQ(nullptr, schema_create(NULL));
    
    // Create another schema
    retldb_schema_t* another_schema = schema_create("ANOTHER_SCHEMA");
    EXPECT_NE(nullptr, another_schema);
    
    // Free it
    schema_free(another_schema);
}

// Test adding fields
TEST_F(SchemaTest, AddField) {
    // Add some fields
    EXPECT_EQ(0, schema_add_field(schema, "id", int_type, 0, NULL));
    EXPECT_EQ(0, schema_add_field(schema, "name", string_type, 1, NULL));
    
    // Try to add a field with the same name (should fail)
    EXPECT_NE(0, schema_add_field(schema, "id", string_type, 1, NULL));
    
    // Add more fields
    EXPECT_EQ(0, schema_add_field(schema, "age", int_type, 1, NULL));
    EXPECT_EQ(0, schema_add_field(schema, "email", string_type, 1, NULL));
    
    // Check field count
    EXPECT_EQ(4, schema_get_field_count(schema));
}

// Test getting fields
TEST_F(SchemaTest, GetField) {
    // Add some fields
    EXPECT_EQ(0, schema_add_field(schema, "id", int_type, 0, NULL));
    EXPECT_EQ(0, schema_add_field(schema, "name", string_type, 1, NULL));
    
    // Get fields by name
    EXPECT_NE(nullptr, schema_get_field(schema, "id"));
    EXPECT_NE(nullptr, schema_get_field(schema, "name"));
    EXPECT_EQ(nullptr, schema_get_field(schema, "non_existent"));
    
    // Get fields by index
    EXPECT_NE(nullptr, schema_get_field_by_index(schema, 0));
    EXPECT_NE(nullptr, schema_get_field_by_index(schema, 1));
    EXPECT_EQ(nullptr, schema_get_field_by_index(schema, 2)); // Out of bounds
    EXPECT_EQ(nullptr, schema_get_field_by_index(schema, -1)); // Invalid index
}

// Test schema validation
TEST_F(SchemaTest, ValidateSchema) {
    // Empty schema (should be invalid)
    EXPECT_NE(0, schema_validate(schema));
    
    // Add a field
    EXPECT_EQ(0, schema_add_field(schema, "id", int_type, 0, NULL));
    
    // Now it should be valid
    EXPECT_EQ(0, schema_validate(schema));
    
    // Test with NULL schema (should be invalid)
    EXPECT_NE(0, schema_validate(NULL));
} 