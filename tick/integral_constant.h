/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    integral_constant.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_INTEGRAL_CONSTANT_H
#define TICK_GUARD_INTEGRAL_CONSTANT_H

#include <type_traits>

namespace tick {

template<class T, T v>
struct integral_constant
: std::integral_constant<T, v>
{
    constexpr T operator()() const noexcept { return v; }
};

}

#endif