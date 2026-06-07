#include <beman/str_split/split.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string_view>

using namespace std::literals;
namespace bss = beman::str_split;

TEST(Split, SplitZeroTimesOnSingleValue) {
    auto words = ""sv | bss::views::split(' ') | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(words, testing::ElementsAre());
}

TEST(Split, SplitsOnceOnSingleValue) {
    auto name = "Alice's"sv;
    auto parts = name | bss::views::split('\'') | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(parts, testing::ElementsAre("Alice"sv, "s"sv));
}

TEST(Split, SplitsManyTimesOnSingleValue) {
    auto title = "Down the Rabbit-Hole."sv;
    auto words = title | bss::views::split(' ') | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(words, testing::ElementsAre("Down", "the", "Rabbit-Hole."));
}

TEST(Split, SplitZeroTimesOnEmptyPattern) {
    auto parts = ""sv | bss::views::split(""sv) | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(parts, testing::ElementsAre());
}

TEST(Split, SplitOnceOnEmptyPattern) {
    auto parts = "A"sv
        | bss::views::split(""sv)
        | std::ranges::to<std::vector<std::string>>();

    EXPECT_THAT(parts, testing::ElementsAre("A"));
}

TEST(Split, SplitsManyTimesEmptyPattern) {
    auto parts = "Alice"sv
            | bss::views::split(""sv)
            | std::ranges::to<std::vector<std::string>>();

    EXPECT_THAT(parts, testing::ElementsAre("A"sv, "l"sv, "i"sv, "c"sv, "e"sv));
}

TEST(Split, SplitZeroTimesOnNonEmptyPattern) {
    auto parts = ""sv | bss::views::split("the"sv) | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(parts, testing::ElementsAre());
}

TEST(Split, SplitOnceOnNonEmptyPattern) {
    auto parts = "Down the Rabbit-Hole."sv
        | bss::views::split("the"sv)
        | std::ranges::to<std::vector<std::string>>();

    EXPECT_THAT(parts, testing::ElementsAre("Down "sv, " Rabbit-Hole."sv));
}

TEST(Split, SplitsManyTimesNonEmptyPattern) {
    auto parts = "Alice was beginning to get very tired"sv
            | bss::views::split(" t"sv)
            | std::ranges::to<std::vector<std::string>>();

    EXPECT_THAT(parts, testing::ElementsAre("Alice was beginning"sv, "o get very"sv, "ired"sv));
}
