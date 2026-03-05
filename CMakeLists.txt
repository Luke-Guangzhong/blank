cmake_minimum_required(VERSION 3.10)

project(myapp VERSION 0.0.1 LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ExternalProject)

find_package(LOG4C QUIET)

if(LOG4C_FOUND)
    add_library(liblog4c UNKNOWN IMPORTED)
    set_target_properties(liblog4c PROPERTIES IMPORTED_LOCATION "${LOG4C_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${LOG4C_INCLUDE_DIR}")
    set(BUILD_LOG4C_FROM_SOURCE FALSE)
else()
    set(BUILD_LOG4C_FROM_SOURCE TRUE)
endif()

if(BUILD_LOG4C_FROM_SOURCE)
    include(Log4cExternalProject)
    setup_log4c_from_source()
endif()

externalproject_add(
    external-trower-base64
    PREFIX ${CMAKE_BINARY_DIR}/third-party/trower-base64
    GIT_REPOSITORY https://gitee.com/third_party_2/trower-base64.git
    GIT_TAG "local/1.2.0"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
               -DBUILD_TESTING=OFF
               -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_POSITION_INDEPENDENT_CODE=ON
               -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    UPDATE_COMMAND ""
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/lib/libtrower-base64${CMAKE_STATIC_LIBRARY_SUFFIX})
add_library(libtrower-base64 STATIC IMPORTED GLOBAL)
set_target_properties(libtrower-base64 PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/libtrower-base64${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(libtrower-base64 external-trower-base64)

externalproject_add(
    external-cJSON
    PREFIX ${CMAKE_BINARY_DIR}/third-party/cJSON
    GIT_REPOSITORY https://gitee.com/third_party_2/cJSON.git
    GIT_TAG "v1.7.8"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
               -DENABLE_CJSON_TEST=OFF
               -DBUILD_SHARED_AND_STATIC_LIBS=ON
               -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
               -DENABLE_CJSON_UTILS=ON
               -DCMAKE_POSITION_INDEPENDENT_CODE=ON
               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    UPDATE_COMMAND ""
    BUILD_BYPRODUCTS ${CMAKE_BINARY_DIR}/lib/libcjson${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_BINARY_DIR}/lib/libcjson_utils${CMAKE_STATIC_LIBRARY_SUFFIX})
add_library(libcJSON STATIC IMPORTED GLOBAL)
add_library(libcJSON_utils STATIC IMPORTED GLOBAL)
set_target_properties(libcJSON PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/libcjson${CMAKE_STATIC_LIBRARY_SUFFIX})
set_target_properties(libcJSON_utils PROPERTIES IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/lib/libcjson_utils${CMAKE_STATIC_LIBRARY_SUFFIX})
add_dependencies(libcJSON external-cJSON)
add_dependencies(libcJSON_utils external-cJSON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin)

# Coverage options
option(ENABLE_LLVM_COVERAGE "Enable LLVM native coverage mode" OFF)

# Check compiler and validate coverage option
if(ENABLE_LLVM_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU")
        message(FATAL_ERROR "ENABLE_LLVM_COVERAGE is only supported with Clang/AppleClang. Current compiler: ${CMAKE_C_COMPILER_ID}")
    endif()
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.c src/log.c)
target_include_directories(${PROJECT_NAME} PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib)
target_link_libraries(
    ${PROJECT_NAME}
    libtrower-base64
    libcJSON
    liblog4c)
add_dependencies(
    ${PROJECT_NAME}
    libtrower-base64
    libcJSON
    liblog4c)
target_compile_definitions(${PROJECT_NAME} PRIVATE LOG4C_RCPATH="${CMAKE_CURRENT_SOURCE_DIR}/resource")

if(ENABLE_COVERAGE_TEST)
    if(ENABLE_LLVM_COVERAGE)
        target_compile_options(${PROJECT_NAME} PRIVATE -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(${PROJECT_NAME} PRIVATE -fprofile-instr-generate)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE --coverage)
        target_link_options(${PROJECT_NAME} PRIVATE --coverage)
    endif()
endif()

message(STATUS "")
message(STATUS "=== log4c-example Configuration Summary ===")
if(BUILD_LOG4C_FROM_SOURCE)
    message(STATUS "  log4c    : Building from source")
else()
    message(STATUS "  log4c    : System (${LOG4C_LIBRARY})")
endif()
message(STATUS "==============================================")
message(STATUS "")
