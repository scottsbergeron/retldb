# Example applications
add_executable(simple_read simple_read.c)
target_link_libraries(simple_read PRIVATE retldb)

add_executable(bulk_load bulk_load.c)
target_link_libraries(bulk_load PRIVATE retldb)

# Install examples
install(TARGETS simple_read bulk_load
    RUNTIME DESTINATION bin/examples
) 