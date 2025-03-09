# FindSnappy.cmake - Find Snappy library
#
# This module defines:
#  SNAPPY_FOUND - True if Snappy is found
#  SNAPPY_INCLUDE_DIRS - The Snappy include directories
#  SNAPPY_LIBRARIES - The Snappy libraries
#  SNAPPY_VERSION - The Snappy version string
#
# and the following imported target:
#  Snappy::Snappy - The Snappy library

# Try to find Snappy using pkg-config first
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_SNAPPY QUIET snappy)
endif()

# Find the include directory
find_path(SNAPPY_INCLUDE_DIR
  NAMES snappy.h
  PATHS
    ${PC_SNAPPY_INCLUDE_DIRS}
    /usr/include
    /usr/local/include
    /opt/local/include
    $ENV{SNAPPY_DIR}/include
  PATH_SUFFIXES snappy
)

# Find the library
if(WIN32)
  # For Windows, try to find the library with different possible names
  find_library(SNAPPY_LIBRARY
    NAMES snappy libsnappy snappy_static libsnappy_static
    PATHS
      ${PC_SNAPPY_LIBRARY_DIRS}
      $ENV{SNAPPY_DIR}/lib
      $ENV{VCPKG_INSTALLATION_ROOT}/installed/x64-windows/lib
      $ENV{VCPKG_INSTALLATION_ROOT}/installed/x86-windows/lib
  )
else()
  # For Unix-like systems
  find_library(SNAPPY_LIBRARY
    NAMES snappy libsnappy
    PATHS
      ${PC_SNAPPY_LIBRARY_DIRS}
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      $ENV{SNAPPY_DIR}/lib
  )
endif()

# Set version
set(SNAPPY_VERSION ${PC_SNAPPY_VERSION})

# Handle the QUIETLY and REQUIRED arguments and set SNAPPY_FOUND
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Snappy
  REQUIRED_VARS SNAPPY_LIBRARY SNAPPY_INCLUDE_DIR
  VERSION_VAR SNAPPY_VERSION
)

# Set output variables
if(SNAPPY_FOUND)
  set(SNAPPY_LIBRARIES ${SNAPPY_LIBRARY})
  set(SNAPPY_INCLUDE_DIRS ${SNAPPY_INCLUDE_DIR})
  
  # Create imported target
  if(NOT TARGET Snappy::Snappy)
    add_library(Snappy::Snappy UNKNOWN IMPORTED)
    set_target_properties(Snappy::Snappy PROPERTIES
      IMPORTED_LOCATION "${SNAPPY_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${SNAPPY_INCLUDE_DIR}"
    )
  endif()
endif()

# Mark as advanced
mark_as_advanced(SNAPPY_INCLUDE_DIR SNAPPY_LIBRARY) 