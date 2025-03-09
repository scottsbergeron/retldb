#include "retldb/types.h"
#include <gtest/gtest.h>
#include <string.h>

// Test basic type creation
TEST(TypesTest, CreateBasicTypes) {
    // Create a boolean type
    retldb_type bool_type = retldb_type_create(RETLDB_TYPE_BOOLEAN, RETLDB_TYPE_FLAG_NONE);
    EXPECT_EQ(bool_type.id, RETLDB_TYPE_BOOLEAN);
    EXPECT_EQ(bool_type.flags, RETLDB_TYPE_FLAG_NONE);
    
    // Create an integer type with nullable flag
    retldb_type int_type = retldb_type_create(RETLDB_TYPE_INT32, RETLDB_TYPE_FLAG_NULLABLE);
    EXPECT_EQ(int_type.id, RETLDB_TYPE_INT32);
    EXPECT_EQ(int_type.flags, RETLDB_TYPE_FLAG_NULLABLE);
    
    // Create a string type with primary key flag
    retldb_type string_type = retldb_type_create(RETLDB_TYPE_STRING, RETLDB_TYPE_FLAG_PRIMARY);
    EXPECT_EQ(string_type.id, RETLDB_TYPE_STRING);
    EXPECT_EQ(string_type.flags, RETLDB_TYPE_FLAG_PRIMARY);
}

// Test decimal type creation
TEST(TypesTest, CreateDecimalType) {
    retldb_type decimal_type = retldb_type_create_decimal(10, 2, RETLDB_TYPE_FLAG_NONE);
    EXPECT_EQ(decimal_type.id, RETLDB_TYPE_DECIMAL);
    EXPECT_EQ(decimal_type.flags, RETLDB_TYPE_FLAG_NONE);
    EXPECT_EQ(decimal_type.info.decimal.precision, 10);
    EXPECT_EQ(decimal_type.info.decimal.scale, 2);
}

// Test array type creation
TEST(TypesTest, CreateArrayType) {
    retldb_type element_type = retldb_type_create(RETLDB_TYPE_INT32, RETLDB_TYPE_FLAG_NONE);
    retldb_type array_type = retldb_type_create_array(element_type, RETLDB_TYPE_FLAG_NULLABLE);
    
    EXPECT_EQ(array_type.id, RETLDB_TYPE_ARRAY);
    EXPECT_EQ(array_type.flags, RETLDB_TYPE_FLAG_NULLABLE);
    EXPECT_EQ(array_type.info.array.element_type->id, RETLDB_TYPE_INT32);
    
    // Clean up
    retldb_type_free(&array_type);
}

// Test map type creation
TEST(TypesTest, CreateMapType) {
    retldb_type key_type = retldb_type_create(RETLDB_TYPE_STRING, RETLDB_TYPE_FLAG_NONE);
    retldb_type value_type = retldb_type_create(RETLDB_TYPE_DOUBLE, RETLDB_TYPE_FLAG_NULLABLE);
    
    retldb_type map_type = retldb_type_create_map(key_type, value_type, RETLDB_TYPE_FLAG_NONE);
    
    EXPECT_EQ(map_type.id, RETLDB_TYPE_MAP);
    EXPECT_EQ(map_type.flags, RETLDB_TYPE_FLAG_NONE);
    EXPECT_EQ(map_type.info.map.key_type->id, RETLDB_TYPE_STRING);
    EXPECT_EQ(map_type.info.map.value_type->id, RETLDB_TYPE_DOUBLE);
    EXPECT_EQ(map_type.info.map.value_type->flags, RETLDB_TYPE_FLAG_NULLABLE);
    
    // Clean up
    retldb_type_free(&map_type);
}

// Test struct type creation
TEST(TypesTest, CreateStructType) {
    // Create field types
    retldb_type id_type = retldb_type_create(RETLDB_TYPE_INT64, RETLDB_TYPE_FLAG_PRIMARY);
    retldb_type name_type = retldb_type_create(RETLDB_TYPE_STRING, RETLDB_TYPE_FLAG_NULLABLE);
    
    // Create fields
    retldb_field fields[2];
    
    fields[0].name = strdup("id");
    fields[0].type = malloc(sizeof(retldb_type));
    *fields[0].type = id_type;
    
    fields[1].name = strdup("name");
    fields[1].type = malloc(sizeof(retldb_type));
    *fields[1].type = name_type;
    
    // Create struct type
    retldb_type struct_type = retldb_type_create_struct(fields, 2, RETLDB_TYPE_FLAG_NONE);
    
    EXPECT_EQ(struct_type.id, RETLDB_TYPE_STRUCT);
    EXPECT_EQ(struct_type.flags, RETLDB_TYPE_FLAG_NONE);
    EXPECT_EQ(struct_type.info.structure.field_count, 2);
    EXPECT_STREQ(struct_type.info.structure.fields[0].name, "id");
    EXPECT_EQ(struct_type.info.structure.fields[0].type->id, RETLDB_TYPE_INT64);
    EXPECT_STREQ(struct_type.info.structure.fields[1].name, "name");
    EXPECT_EQ(struct_type.info.structure.fields[1].type->id, RETLDB_TYPE_STRING);
    
    // Clean up
    retldb_type_free(&struct_type);
    
    // Clean up original fields (since we made copies in create_struct)
    free(fields[0].name);
    free(fields[0].type);
    free(fields[1].name);
    free(fields[1].type);
}

// Test value creation
TEST(TypesTest, CreateValue) {
    // Create a type
    retldb_type int_type = retldb_type_create(RETLDB_TYPE_INT32, RETLDB_TYPE_FLAG_NONE);
    
    // Create a value
    retldb_value value = retldb_value_create(int_type);
    
    EXPECT_EQ(value.type.id, RETLDB_TYPE_INT32);
    EXPECT_FALSE(value.is_null);
    
    // Create a NULL value
    retldb_value null_value = retldb_value_create_null(int_type);
    
    EXPECT_EQ(null_value.type.id, RETLDB_TYPE_INT32);
    EXPECT_TRUE(null_value.is_null);
}

// Test schema creation
TEST(TypesTest, CreateSchema) {
    // Create field types
    retldb_type id_type = retldb_type_create(RETLDB_TYPE_INT64, RETLDB_TYPE_FLAG_PRIMARY);
    retldb_type name_type = retldb_type_create(RETLDB_TYPE_STRING, RETLDB_TYPE_FLAG_NULLABLE);
    retldb_type age_type = retldb_type_create(RETLDB_TYPE_INT32, RETLDB_TYPE_FLAG_NONE);
    
    // Create fields
    retldb_schema_field fields[3];
    
    fields[0].name = strdup("id");
    fields[0].type = id_type;
    fields[0].description = strdup("Primary key");
    
    fields[1].name = strdup("name");
    fields[1].type = name_type;
    fields[1].description = strdup("User name");
    
    fields[2].name = strdup("age");
    fields[2].type = age_type;
    fields[2].description = strdup("User age");
    
    // Create primary key indices
    uint32_t primary_key_indices[1] = {0}; // id is the primary key
    
    // Create schema
    retldb_schema schema = retldb_schema_create("users", fields, 3, primary_key_indices, 1);
    
    EXPECT_STREQ(schema.name, "users");
    EXPECT_EQ(schema.field_count, 3);
    EXPECT_EQ(schema.primary_key_count, 1);
    EXPECT_EQ(schema.primary_key_indices[0], 0);
    
    EXPECT_STREQ(schema.fields[0].name, "id");
    EXPECT_EQ(schema.fields[0].type.id, RETLDB_TYPE_INT64);
    EXPECT_STREQ(schema.fields[0].description, "Primary key");
    
    EXPECT_STREQ(schema.fields[1].name, "name");
    EXPECT_EQ(schema.fields[1].type.id, RETLDB_TYPE_STRING);
    EXPECT_STREQ(schema.fields[1].description, "User name");
    
    EXPECT_STREQ(schema.fields[2].name, "age");
    EXPECT_EQ(schema.fields[2].type.id, RETLDB_TYPE_INT32);
    EXPECT_STREQ(schema.fields[2].description, "User age");
    
    // Clean up
    retldb_schema_free(&schema);
    
    // Clean up original fields (since we made copies in create_schema)
    for (int i = 0; i < 3; i++) {
        free(fields[i].name);
        free(fields[i].description);
    }
}

// Test schema field validation
TEST(TypesTest, ValidateSchemaField) {
    // Create a field
    retldb_type int_type = retldb_type_create(RETLDB_TYPE_INT32, RETLDB_TYPE_FLAG_NONE);
    retldb_schema_field field;
    field.name = strdup("age");
    field.type = int_type;
    field.description = strdup("User age");
    
    // Create a matching value
    retldb_value int_value = retldb_value_create(int_type);
    int_value.value.int32 = 25;
    
    // Create a non-matching value
    retldb_type string_type = retldb_type_create(RETLDB_TYPE_STRING, RETLDB_TYPE_FLAG_NONE);
    retldb_value string_value = retldb_value_create(string_type);
    
    // Create a NULL value
    retldb_value null_value = retldb_value_create_null(int_type);
    
    // Validate
    EXPECT_TRUE(retldb_schema_validate_field(&int_value, &field));
    EXPECT_FALSE(retldb_schema_validate_field(&string_value, &field));
    EXPECT_FALSE(retldb_schema_validate_field(&null_value, &field));
    
    // Clean up
    retldb_value_free(&int_value);
    retldb_value_free(&string_value);
    retldb_value_free(&null_value);
    free(field.name);
    free(field.description);
}

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
