// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/str_split/config.hpp>
#include <beman/str_split/split_when.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string_view>

using namespace std::literals;
namespace bss = beman::str_split;

TEST(SplitWhen, SplitZeroTimesUsingStdRangesSearch) {
    auto searcher = [](auto&& view){ return std::ranges::search(view, ":"sv); };
    auto words = ""sv | bss::views::split_when(searcher) | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(words, testing::ElementsAre());
}

TEST(SplitWhen, SplitsOnceUsingStdRangesSearch) {
    auto searcher = [](auto&& view){ return std::ranges::search(view, "'s"sv); };
    auto name = "Alice's"sv;
    auto parts = name | bss::views::split_when(searcher) | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(parts, testing::ElementsAre("Alice"sv, ""sv));
}

TEST(SplitWhen, SplitsManyTimesUsingStdRangesSearch) {
    auto title = "Down the Rabbit-Hole."sv;
    auto searcher = [](auto&& view){ return std::ranges::search(view, " "sv); };
    auto words = title | bss::views::split_when(searcher) | std::ranges::to<std::vector<std::string>>();
    EXPECT_THAT(words, testing::ElementsAre("Down", "the", "Rabbit-Hole."));
}
