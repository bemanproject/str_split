// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_STR_SPLIT_STR_SPLIT_HPP
#define BEMAN_STR_SPLIT_STR_SPLIT_HPP

#include <beman/str_split/config.hpp>

#if BEMAN_STR_SPLIT_USE_MODULES() && !defined(BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT)

import beman.str_split;

#else

    #include <beman/str_split/str_split_to.hpp>

#endif // BEMAN_STR_SPLIT_USE_MODULES() &&
       // !defined(BEMAN_STR_SPLIT_INCLUDED_FROM_INTERFACE_UNIT)

#endif // BEMAN_STR_SPLIT_STR_SPLIT_HPP
