// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/str_split/split_when.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string_view>

using namespace std::literals;
namespace bss = beman::str_split;

namespace {

class any_seq_of {
public:
    constexpr explicit any_seq_of(std::string_view delimiters)
        : delimiters_(delimiters)
    {
    }

    template<class V>
        requires std::constructible_from<std::string_view, V>
    V operator()(V view)
    {
        auto str = std::string_view(view);
        auto begin_pos = str.find_first_of(delimiters_);
        if (begin_pos == str.npos) {
            return {std::ranges::end(view), std::ranges::end(view)};
        }
        auto end_pos = str.find_first_not_of(delimiters_, begin_pos + 1);
        return {std::ranges::begin(view) + begin_pos, std::ranges::begin(view) + end_pos};
    }

private:
    std::string_view delimiters_;
};

}

TEST(SplitWhen, SplitZeroTimes) {
    auto words = ""sv
            | bss::views::split_when(any_seq_of(" \t"sv))
            | std::ranges::to<std::vector<std::string>>();

    EXPECT_THAT(words, testing::ElementsAre());
}

TEST(SplitWhen, SplitsOnce) {
    auto parts = "Down the Rabbit-Hole"sv
        | bss::views::split_when(any_seq_of("-."sv))
        | std::ranges::to<std::vector<std::string>>();

    EXPECT_THAT(parts, testing::ElementsAre("Down the Rabbit"sv, "Hole"sv));
}

TEST(SplitWhen, SplitsManyTimes) {
    auto words = "Alice \t was beginning to\tget \tvery    tired"sv
            | bss::views::split_when(any_seq_of(" \t"sv))
            | std::ranges::to<std::vector<std::string>>();

    EXPECT_THAT(words, testing::ElementsAre("Alice", "was", "beginning", "to", "get", "very", "tired"));
}
