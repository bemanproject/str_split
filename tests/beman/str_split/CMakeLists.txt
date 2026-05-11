# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

add_executable(beman.str_split.tests.str_split_to)
target_sources(beman.str_split.tests.str_split_to PRIVATE str_split_to.test.cpp)
target_link_libraries(
    beman.str_split.tests.str_split_to
    PRIVATE beman::str_split GTest::gtest_main GTest::gmock
)
if(BEMAN_EXEMPLAR_USE_MODULES)
    set_target_properties(
        beman.str_split.tests.str_split_to
        PROPERTIES CXX_MODULE_STD ON
    )
endif()

include(GoogleTest)
gtest_discover_tests(beman.str_split.tests.str_split_to DISCOVERY_TIMEOUT 60)
