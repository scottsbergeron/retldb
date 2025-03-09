# Source files
set(RETLDB_SOURCES
    common/error.c
    common/db.c
    storage/file.c
    storage/mmap.c
    storage/buffer.c
    types/datatype.c
    types/schema.c
)

# Create the library
add_library(retldb ${RETLDB_SOURCES})

# Link dependencies
target_link_libraries(retldb PRIVATE
    LZ4::LZ4
    Snappy::Snappy
)

# Set include directories
target_include_directories(retldb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Install the library
install(TARGETS retldb
    EXPORT retldb-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install the export
install(EXPORT retldb-targets
    FILE retldb-targets.cmake
    NAMESPACE retldb::
    DESTINATION lib/cmake/retldb
)