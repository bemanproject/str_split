// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_STR_SPLIT_TODO_HPP
#define BEMAN_STR_SPLIT_TODO_HPP

#include <beman/str_split/config.hpp>

#include <string>
#include <string_view>

#if BEMAN_STR_SPLIT_USE_MODULES() && !defined(BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT)

import beman.str_split;

#else

namespace beman::str_split {

//------------------------------------------------------------------------------
// Patterns:
//------------------------------------------------------------------------------

// Splits by a substring.
struct split_by {
    // Constructor for anything that can be converted to a `std::string_view`.
    constexpr explicit split_by(std::string_view delimiter) : delimiter_(delimiter) {}

    // Constructor for the single character case.
    constexpr explicit split_by(char delimiter) : delimiter_(1, delimiter) {}

    // Constructor for range of characters that are not `std::string_view` convertible.
    template <std::ranges::input_range Range>
        requires std::same_as<std::ranges::range_value_t<Range>, char> &&
                 (!std::convertible_to<Range &&, std::string_view>) &&
                 (!std::same_as<std::remove_cvref_t<Range>, split_by>)
    explicit split_by(Range&& range) : delimiter_(std::ranges::begin(range), std::ranges::end(range)) {}

    // TODO(aryann): Here and below, implement a find member function that accepts the current "haystack" string and
    // returns the position of the first match. We may also need to control the visibility of such function.

  private:
    std::string delimiter_;
};

// Splits by the first matching character in a given character sequence.
struct split_by_first_of {

  private:
    std::string chars_;
};

struct split_by_ascii_whitespace {};

//------------------------------------------------------------------------------
// Split functions:
//------------------------------------------------------------------------------

// TODO(aryann): Consider an alternative approach where the split type is determined by the function name:
//
//   * split(std::string_view): Equivalent to
//     str_split(std::string_view, split_by_ascii_whitespace).
//
//   * split(std::string_view, Range&&): Equivalent to
//     str_split(std::string_view, split_by).
//
//   * split_by_first_of(std::string_view, Range&&): Equivalent to
//     str_split(std::string_view, split_by_first_of).
//
// I'm partial towards this alternative as it more closely mirrors `std::string::find` and `:find_first_of`.

// TODO(aryann): We should structure the input similar to `split_by` by allowing both ranges and
// `std::string_view`-convertible types.

template <typename Container, typename Pattern>
constexpr Container str_split(std::string_view input, Pattern pattern) {
    Container empty;
    return empty;
}

template <typename Container>
constexpr Container str_split(std::string_view input) {
    return str_split<Container>(input, split_by_ascii_whitespace{});
}

// TODO(aryann): Add support for max splits.

} // namespace beman::str_split

#endif // BEMAN_STR_SPLIT_USE_MODULES() &&
       // !defined(BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT)

#endif // BEMAN_STR_SPLIT_TODO_HPP
