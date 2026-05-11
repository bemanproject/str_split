// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/str_split/config.hpp>
#include <beman/str_split/str_split_to.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#if BEMAN_STR_SPLIT_USE_MODULES()

import std;

#else

    #include <array>
    #include <deque>
    #include <string_view>
    #include <type_traits>
    #include <vector>

#endif // BEMAN_STR_SPLIT_USE_MODULES()

namespace {

using namespace std::literals::string_view_literals;

using ::beman::str_split::split_by;
using ::beman::str_split::split_by_ascii_whitespace;
using ::beman::str_split::split_by_char;
using ::beman::str_split::str_split_to;
using ::testing::ElementsAre;

TEST(Delimiter, SplitBy) {
    // `std:string-view`-convertible inputs:
    EXPECT_THAT(str_split_to("my string"sv, split_by(" ")), ElementsAre());
    EXPECT_THAT(str_split_to("my string"sv, split_by("string")), ElementsAre());
    EXPECT_THAT(str_split_to("my string"sv, split_by(std::string("string"))), ElementsAre());
    EXPECT_THAT(str_split_to("my string"sv, split_by(std::string_view("string"))), ElementsAre());

    // Ranges:
    EXPECT_THAT(str_split_to("my string"sv, split_by(std::vector<char>{'a', 'b', 'c'})), ElementsAre());
    EXPECT_THAT(str_split_to("my string"sv, split_by(std::array{'a', 'b', 'c'})), ElementsAre());
}

TEST(Delimiter, SplitByChar) {
    EXPECT_THAT(str_split_to("my string"sv, split_by_char(' ')), ElementsAre());
    EXPECT_THAT(str_split_to("my string"sv, split_by_char('s')), ElementsAre());
}

TEST(Delimiter, SplitByAsciiWhitespace) {
    EXPECT_THAT(str_split_to(""sv, split_by_ascii_whitespace()), ElementsAre());
    EXPECT_THAT(str_split_to("my string"sv, split_by_ascii_whitespace()), ElementsAre());
}

TEST(StrSplitTo, StringViewVector) {
    static_assert(std::is_same_v<decltype(str_split_to("my string"sv, split_by_ascii_whitespace())),
                                 std::vector<std::string_view>>);

    EXPECT_THAT(str_split_to("my string"sv, split_by_ascii_whitespace()), ElementsAre());
}

TEST(StrSplitTo, StringViewContainer) {
    static_assert(std::is_same_v<decltype(str_split_to<std::vector<std::string_view>>("my string"sv,
                                                                                      split_by_ascii_whitespace())),
                                 std::vector<std::string_view>>);
    static_assert(std::is_same_v<decltype(str_split_to<std::deque<std::string_view>>("my string"sv,
                                                                                     split_by_ascii_whitespace())),
                                 std::deque<std::string_view>>);

    EXPECT_THAT(str_split_to<std::vector<std::string_view>>("my string"sv, split_by_ascii_whitespace()),
                ElementsAre());
    EXPECT_THAT(str_split_to<std::deque<std::string_view>>("my string"sv, split_by_ascii_whitespace()), ElementsAre());
}

TEST(StrSplitTo, StringContainer) {
    static_assert(
        std::is_same_v<decltype(str_split_to<std::vector<std::string>>("my string"sv, split_by_ascii_whitespace())),
                       std::vector<std::string>>);
    static_assert(
        std::is_same_v<decltype(str_split_to<std::deque<std::string>>("my string"sv, split_by_ascii_whitespace())),
                       std::deque<std::string>>);

    EXPECT_THAT(str_split_to<std::vector<std::string>>("my string"sv, split_by_ascii_whitespace()), ElementsAre());
    EXPECT_THAT(str_split_to<std::deque<std::string>>("my string"sv, split_by_ascii_whitespace()), ElementsAre());
}

TEST(StrSplitTo, CharTypes) {
    static_assert(std::is_same_v<decltype(str_split_to(std::wstring_view(L"my string"), split_by_ascii_whitespace())),
                                 std::vector<std::wstring_view>>);
    static_assert(
        std::is_same_v<decltype(str_split_to(std::u8string_view(u8"my string"), split_by_ascii_whitespace())),
                       std::vector<std::u8string_view>>);
    static_assert(
        std::is_same_v<decltype(str_split_to(std::u16string_view(u"my string"), split_by_ascii_whitespace())),
                       std::vector<std::u16string_view>>);
    static_assert(
        std::is_same_v<decltype(str_split_to(std::u32string_view(U"my string"), split_by_ascii_whitespace())),
                       std::vector<std::u32string_view>>);

    EXPECT_THAT(str_split_to(std::wstring_view(L"my string"), split_by_ascii_whitespace()), ElementsAre());
    EXPECT_THAT(str_split_to(std::u8string_view(u8"my string"), split_by_ascii_whitespace()), ElementsAre());
    EXPECT_THAT(str_split_to(std::u16string_view(u"my string"), split_by_ascii_whitespace()), ElementsAre());
    EXPECT_THAT(str_split_to(std::u32string_view(U"my string"), split_by_ascii_whitespace()), ElementsAre());
}

} // namespace
