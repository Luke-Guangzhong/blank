# ExpatExternalProject.cmake - Define function to build expat from source
#
# Provides: setup_expat_from_source() - Configure and add expat ExternalProject

include(ExternalProject)

function(setup_expat_from_source)
    # expat 1.95.8 Note: log4c 1.2.4 requires expat >= 1.95.1 System library may be expat 2.x, which has API incompatibility with log4c 1.2.4

    externalproject_add(
        expat
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third-party/libexpat
        GIT_REPOSITORY https://gitee.com/third_party_2/libexpat.git
        GIT_TAG R_1_95_8
        GIT_REMOTE_NAME origin
        GIT_PROGRESS TRUE
        UPDATE_COMMAND ""
        PATCH_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/build-expat.sh <SOURCE_DIR>/expat
        CONFIGURE_COMMAND <SOURCE_DIR>/expat/configure --prefix=${CMAKE_CURRENT_BINARY_DIR} --host=${CMAKE_HOST_SYSTEM_PROCESSOR}
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_IN_SOURCE 1
        BUILD_BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}expat${CMAKE_STATIC_LIBRARY_SUFFIX}")

    add_library(libexpat STATIC IMPORTED)
    set_target_properties(libexpat PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}expat${CMAKE_STATIC_LIBRARY_SUFFIX}")
    add_dependencies(libexpat expat)

    message(STATUS "Configured expat to build from source")
endfunction()
