# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.30...4.3)

include(infra/cmake/enable-experimental-import-std.cmake)

project(
    beman.str_split
    DESCRIPTION "Stand-alone, easy-to-use string split utilities"
    LANGUAGES CXX
    VERSION 0.1.0
)

# [CMAKE.SKIP_TESTS]
option(
    BEMAN_STR_SPLIT_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# [CMAKE.SKIP_EXAMPLES]
option(
    BEMAN_STR_SPLIT_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

option(BEMAN_STR_SPLIT_USE_MODULES "Provide beman.str_split as a C++ module" OFF)

if(BEMAN_STR_SPLIT_USE_MODULES)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/include/beman/str_split/config_generated.hpp.in"
    "${PROJECT_BINARY_DIR}/include/beman/str_split/config_generated.hpp"
    @ONLY
)

# for find of beman_install_library and configure_build_telemetry
include(infra/cmake/beman-install-library.cmake)
include(infra/cmake/BuildTelemetryConfig.cmake)

if(BEMAN_STR_SPLIT_USE_MODULES)
    add_library(beman.str_split STATIC)
else()
    add_library(beman.str_split INTERFACE)
endif()
add_library(beman::str_split ALIAS beman.str_split)

if(BEMAN_STR_SPLIT_USE_MODULES)
    target_sources(
        beman.str_split
        PUBLIC
            FILE_SET CXX_MODULES
            FILE_SET HEADERS
                BASE_DIRS
                    "${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
    set_target_properties(beman.str_split PROPERTIES CXX_MODULE_STD ON)
    target_compile_features(beman.str_split PUBLIC cxx_std_23)
else()
    target_sources(
        beman.str_split
        PUBLIC
            FILE_SET HEADERS
                BASE_DIRS
                    "${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${CMAKE_CURRENT_BINARY_DIR}/include"
    )
    set_target_properties(
        beman.str_split
        PROPERTIES VERIFY_INTERFACE_HEADER_SETS ${PROJECT_IS_TOP_LEVEL}
    )
endif()

add_subdirectory(include/beman/str_split)

beman_install_library(beman.str_split TARGETS beman.str_split)
configure_build_telemetry()

if(BEMAN_STR_SPLIT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/beman/str_split)
endif()

if(BEMAN_STR_SPLIT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
