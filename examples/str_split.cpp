// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/str_split/config.hpp>
#include <beman/str_split/str_split.hpp>

#include <iostream>
#include <string_view>

using namespace std::literals::string_view_literals;

using ::beman::str_split::split_by_ascii_whitespace;
using ::beman::str_split::str_split_to;

int main() {
    constexpr std::string_view text = "The quick brown fox jumps over the lazy dog";

    const std::vector<std::string_view> parts = str_split_to(text, split_by_ascii_whitespace());
    for (std::string_view part : parts) {
        std::cout << part << std::endl;
    }

    return 0;
}
