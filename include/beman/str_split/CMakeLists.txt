# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

if(BEMAN_STR_SPLIT_USE_MODULES)
    target_sources(
        beman.str_split
        PUBLIC
            FILE_SET CXX_MODULES FILES str_split.cppm
            FILE_SET HEADERS
                FILES
                    config.hpp
                    str_split.hpp
                    todo.hpp
                    split_when.hpp
                    "${PROJECT_BINARY_DIR}/include/beman/str_split/config_generated.hpp"
    )
else()
    target_sources(
        beman.str_split
        PUBLIC
            FILE_SET HEADERS
                FILES
                    config.hpp
                    str_split.hpp
                    todo.hpp
                    split_when.hpp
                    "${PROJECT_BINARY_DIR}/include/beman/str_split/config_generated.hpp"
    )
endif()
