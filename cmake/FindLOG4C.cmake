find_path(
    LOG4C_INCLUDE_DIR
    NAMES log4c.h
    PATH_SUFFIXES include log4c
    HINTS ${CMAKE_PREFIX_PATH}
    PATHS /usr/local/include /usr/include
    DOC "log4c include directory"
    NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)

find_library(
    LOG4C_LIBRARY
    NAMES log4c
    PATH_SUFFIXES lib
    HINTS ${CMAKE_PREFIX_PATH}
    PATHS /usr/local/lib /usr/lib /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
    DOC "log4c library"
    NO_CMAKE_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH NO_SYSTEM_ENVIRONMENT_PATH)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LOG4C REQUIRED_VARS LOG4C_INCLUDE_DIR LOG4C_LIBRARY)

if(LOG4C_FOUND)
    message(STATUS "Found system log4c: ${LOG4C_LIBRARY}")
else()
    message(STATUS "System log4c not found")
endif()
