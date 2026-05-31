module;

#include <concepts>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

export module beman.str_split;

#define BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT
export {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#include <beman/str_split/str_split.hpp>
#pragma clang diagnostic pop
}
