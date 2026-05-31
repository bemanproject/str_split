# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_EXAMPLES str_split)
message("Examples to be built: ${ALL_EXAMPLES}")

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.str_split.examples.${example})
    target_sources(beman.str_split.examples.${example} PRIVATE ${example}.cpp)
    target_link_libraries(
        beman.str_split.examples.${example}
        PRIVATE beman::str_split
    )
    if(BEMAN_STR_SPLIT_USE_MODULES)
        set_target_properties(
            beman.str_split.examples.${example}
            PROPERTIES CXX_MODULE_STD ON
        )
    endif()
endforeach()
