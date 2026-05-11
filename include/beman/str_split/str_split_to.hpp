// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_STR_SPLIT_STR_SPLIT_TO_HPP
#define BEMAN_STR_SPLIT_STR_SPLIT_TO_HPP

#include <beman/str_split/config.hpp>

#if BEMAN_STR_SPLIT_USE_MODULES() && !defined(BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT)

import beman.str_split;

#else

    #if !BEMAN_STR_SPLIT_USE_MODULES()

        #include <string>
        #include <string_view>

    #endif // !BEMAN_STR_SPLIT_USE_MODULES()

namespace beman::str_split {

// TODO(aryann): Delete this file and move the contents to str_split.hpp once the we make more progress on the surface
// definition.

//------------------------------------------------------------------------------
// Concepts:
//------------------------------------------------------------------------------

// TODO(aryann): Should the concepts be placed in a private namespace?

// A range of chars.
template <typename T>
concept char_range = std::ranges::input_range<T> && std::same_as<std::ranges::range_value_t<T>, char>;

// A type that cannot be converted to `std::string_view`.
template <typename T>
concept not_string_view_convertible = !std::convertible_to<T&&, std::string_view>;

template <typename T, typename Self>
concept different_from = !std::same_as<std::remove_cvref_t<T>, Self>;

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
        requires(
            // Ensures the range's value type is `char`. Notably, this rejects ranges of other values types such as
            // `int` and `unsigned char`. This requirement prevents narrowing conversions.
            char_range<Range> &&

            // Ensures this constructor does not compete with the `std::string_view` overload.
            not_string_view_convertible<Range> &&

            // Ensures this constructor does not hijack copy and move construction which would fail to compile with a
            // difficult-to-read wall of errors.
            different_from<Range, split_by>)
    constexpr explicit split_by(Range&& range) : delimiter_(std::ranges::begin(range), std::ranges::end(range)) {}

    // TODO(aryann): Here and below, implement a find member function that accepts the current "haystack" string and
    // returns the position of the first match. We may also need to control the visibility of such function.

  private:
    const std::string delimiter_;
};

// Splits by the first matching character in a given character sequence.
struct split_by_first_of {

  private:
    const std::string chars_;
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

// TODO(aryann): For now, the caller must pass the return type. We should decide whether to use implicit conversions to
// adapt to the user's declared return type (similar to absl's approach). If we retain the current structure, we should
// decide on a default for the common case (`std::vector<std::string_view>`?).

} // namespace beman::str_split

#endif // BEMAN_STR_SPLIT_USE_MODULES() &&
       // !defined(BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT)

#endif // BEMAN_STR_SPLIT_STR_SPLIT_TO_HPP
