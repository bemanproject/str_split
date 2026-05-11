// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <string_view>
#include <vector>

#include <beman/str_split/config.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <beman/str_split/str_split_to.hpp>

namespace {

using ::beman::str_split::split_by;
using ::beman::str_split::str_split;
using ::testing::ElementsAre;

TEST(StrSplit, SplitBy) {
    using Container = std::vector<std::string_view>;

    // `std:string-view`-convertible inputs:
    EXPECT_THAT(str_split<Container>("my string", split_by(" ")), ElementsAre());
    EXPECT_THAT(str_split<Container>("my string", split_by("string")), ElementsAre());
    EXPECT_THAT(str_split<Container>("my string", split_by(std::string("string"))), ElementsAre());
    EXPECT_THAT(str_split<Container>("my string", split_by(std::string_view("string"))), ElementsAre());

    // Single char:
    EXPECT_THAT(str_split<Container>("my string", split_by('s')), ElementsAre());

    // Ranges:
    EXPECT_THAT(str_split<Container>("my string", split_by(std::vector<char>{'a', 'b', 'c'})), ElementsAre());
    EXPECT_THAT(str_split<Container>("my string", split_by(std::array{'a', 'b', 'c'})), ElementsAre());
}

} // namespace
