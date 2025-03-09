# Find GTest
find_package(GTest REQUIRED)

# Test files
set(RETLDB_TEST_SOURCES
    test_main.c
    common/test_error.c
    storage/test_file.c
    storage/test_mmap.c
    storage/test_buffer.c
    types/test_datatype.c
    types/test_schema.c
)

# Create test executable
add_executable(retldb_tests ${RETLDB_TEST_SOURCES})

# Link dependencies
target_link_libraries(retldb_tests
    PRIVATE
        retldb
        GTest::GTest
        GTest::Main
)

# Add tests
add_test(NAME retldb_unit_tests COMMAND retldb_tests)

# Create test coverage target if gcov is available
find_program(GCOV_PATH gcov)
if(GCOV_PATH AND CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(retldb_tests PRIVATE --coverage)
    target_link_libraries(retldb_tests PRIVATE --coverage)
endif() 