# FindLZ4.cmake - Find LZ4 library
#
# This module defines:
#  LZ4_FOUND - True if LZ4 is found
#  LZ4_INCLUDE_DIRS - The LZ4 include directories
#  LZ4_LIBRARIES - The LZ4 libraries
#  LZ4_VERSION - The LZ4 version string

# Try to find LZ4 using pkg-config first
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_LZ4 QUIET liblz4)
endif()

# Find the include directory
find_path(LZ4_INCLUDE_DIR
  NAMES lz4.h
  PATHS
    ${PC_LZ4_INCLUDE_DIRS}
    /usr/include
    /usr/local/include
    /opt/local/include
    $ENV{LZ4_DIR}/include
  PATH_SUFFIXES lz4
)

# Find the library
if(WIN32)
  # For Windows, try to find the library with different possible names
  find_library(LZ4_LIBRARY
    NAMES lz4 liblz4 lz4_static liblz4_static
    PATHS
      ${PC_LZ4_LIBRARY_DIRS}
      $ENV{LZ4_DIR}/lib
      $ENV{VCPKG_INSTALLATION_ROOT}/installed/x64-windows/lib
      $ENV{VCPKG_INSTALLATION_ROOT}/installed/x86-windows/lib
  )
else()
  # For Unix-like systems
  find_library(LZ4_LIBRARY
    NAMES lz4 liblz4
    PATHS
      ${PC_LZ4_LIBRARY_DIRS}
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      $ENV{LZ4_DIR}/lib
  )
endif()

# Set version
set(LZ4_VERSION ${PC_LZ4_VERSION})

# Handle the QUIETLY and REQUIRED arguments and set LZ4_FOUND
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LZ4
  REQUIRED_VARS LZ4_LIBRARY LZ4_INCLUDE_DIR
  VERSION_VAR LZ4_VERSION
)

# Set output variables
if(LZ4_FOUND)
  set(LZ4_LIBRARIES ${LZ4_LIBRARY})
  set(LZ4_INCLUDE_DIRS ${LZ4_INCLUDE_DIR})
endif()

# Mark as advanced
mark_as_advanced(LZ4_INCLUDE_DIR LZ4_LIBRARY) 