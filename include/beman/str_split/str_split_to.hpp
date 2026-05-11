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
        #include <vector>

    #endif // !BEMAN_STR_SPLIT_USE_MODULES()

namespace beman::str_split {

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
  public:
    // Constructor for anything that can be converted to a `std::string_view`.
    constexpr explicit split_by(std::string_view delimiter) : delimiter_(delimiter) {}

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

// Splits by character.
struct split_by_char {
  public:
    constexpr explicit split_by_char(char delimiter) : delimiter_(delimiter) {}

  private:
    const char delimiter_;
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

template <class OutputIt, class CharT, class Traits, class Delimiter>
auto str_split_to(std::basic_string_view<CharT, Traits> text, Delimiter&& delimiter, OutputIt dest) -> OutputIt {
    return dest;
}

template <class Container, class CharT, class Traits, class Delimiter>
auto str_split_to(std::basic_string_view<CharT, Traits> text, Delimiter&& delimiter) -> Container {
    Container container;
    return str_split_to(text, delimiter, container);
}

template <template <class...> class Container, class CharT, class Traits, class Delimiter>
auto str_split_to(std::basic_string_view<CharT, Traits> text, Delimiter&& delimiter)
    -> Container<std::basic_string_view<CharT, Traits> > {
    return str_split_to(text, delimiter);
}

template <class CharT, class Traits, class Delimiter>
auto str_split_to(std::basic_string_view<CharT, Traits> text, Delimiter&& delimiter)
    -> std::vector<std::basic_string_view<CharT, Traits> > {
    std::vector<std::basic_string_view<CharT, Traits> > result;
    return result;
}

// TODO(aryann): Add support for max splits.

} // namespace beman::str_split

#endif // BEMAN_STR_SPLIT_USE_MODULES() &&
       // !defined(BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT)

#endif // BEMAN_STR_SPLIT_STR_SPLIT_TO_HPP
