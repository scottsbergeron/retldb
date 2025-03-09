# Find GTest
find_package(GTest REQUIRED)

# Enable C++ support
enable_language(CXX)

# Test files
set(RETLDB_TEST_SOURCES
    test_main.cpp
    common/test_error.cpp
    storage/test_file.cpp
    storage/test_mmap.cpp
    storage/test_buffer.cpp
    types/test_datatype.cpp
    types/test_schema.cpp
)

# Create test executable
add_executable(retldb_tests ${RETLDB_TEST_SOURCES})
set_target_properties(retldb_tests PROPERTIES LINKER_LANGUAGE CXX)

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

# Add MSVC-specific compiler flags
if(MSVC)
    add_compile_definitions(
        _CRT_SECURE_NO_WARNINGS     # Disable warnings about "unsafe" functions
        _CRT_NONSTDC_NO_DEPRECATE   # Disable warnings about POSIX function names
    )
endif() 