# Log4cExternalProject.cmake - Define function to build log4c from source
#
# Provides: setup_log4c_from_source() - Configure and add log4c ExternalProject

include(ExternalProject)

function(setup_log4c_from_source)
    # Build log4c from master branch Use --without-expat to prevent log4c from linking expat during compilation This allows linking both log4c and expat 1.95.8 in the main program Avoids log4c using
    # system expat 2.x at runtime which causes API incompatibility

    externalproject_add(
        log4c
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/log4c
        GIT_REPOSITORY https://gitee.com/third_party_2/log4c.git
        GIT_TAG master
        GIT_REMOTE_NAME origin
        GIT_PROGRESS TRUE
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh && <SOURCE_DIR>/configure --prefix=${CMAKE_CURRENT_BINARY_DIR} --enable-test --enable-doc --without-expat
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_IN_SOURCE TRUE
        BUILD_BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}log4c${CMAKE_STATIC_LIBRARY_SUFFIX}")

    add_library(liblog4c STATIC IMPORTED)
    set_target_properties(liblog4c PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}log4c${CMAKE_STATIC_LIBRARY_SUFFIX}")
    add_dependencies(liblog4c log4c expat)

    message(STATUS "Configured log4c to build from source")
endfunction()
