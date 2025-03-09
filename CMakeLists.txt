cmake_minimum_required(VERSION 3.15)
project(retldb VERSION 0.1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Options
option(RETLDB_BUILD_TESTS "Build tests" ON)
option(RETLDB_BUILD_EXAMPLES "Build examples" ON)
option(RETLDB_ENABLE_ASAN "Enable Address Sanitizer" OFF)
option(RETLDB_ENABLE_UBSAN "Enable Undefined Behavior Sanitizer" OFF)

# Global compiler flags
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Werror -pedantic)
    
    # Sanitizer options
    if(RETLDB_ENABLE_ASAN)
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
    
    if(RETLDB_ENABLE_UBSAN)
        add_compile_options(-fsanitize=undefined)
        add_link_options(-fsanitize=undefined)
    endif()
elseif(MSVC)
    add_compile_options(/W4 /WX)
endif()

# Include directories
include_directories(include)

# Find dependencies
find_package(LZ4 REQUIRED)
find_package(Snappy REQUIRED)

# Add subdirectories
add_subdirectory(src)

# Tests
if(RETLDB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(RETLDB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
install(DIRECTORY include/ DESTINATION include/retldb)

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/retldb-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/retldb-config-version.cmake"
    DESTINATION
        lib/cmake/retldb
)

# Generate and install pkg-config file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/retldb.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/retldb.pc
    @ONLY
)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/retldb.pc
    DESTINATION
        lib/pkgconfig
) 