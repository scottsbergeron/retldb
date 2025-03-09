# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

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

# Test executables
add_executable(test_types types/test_types.c)
target_link_libraries(test_types retldb ${GTEST_LIBRARIES} pthread)

# Add tests to CTest
add_test(NAME TypesTest COMMAND test_types)

# Main test runner
add_executable(test_main test_main.cpp)
target_link_libraries(test_main ${GTEST_LIBRARIES} pthread)
add_test(NAME MainTest COMMAND test_main) 