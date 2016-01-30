/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_weakly_ordered.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_WEAKLY_ORDERED_H
#define TICK_GUARD_IS_WEAKLY_ORDERED_H

#include <tick/builder.h>

namespace tick {

TICK_TRAIT(is_weakly_ordered)
{
    template<class T>
    static auto req_impl(T&& x) -> valid<
        TICK_RETURNS(x < x, bool),
        TICK_RETURNS(x > x, bool),
        TICK_RETURNS(x <= x, bool),
        TICK_RETURNS(x >= x, bool)
    >;

    template<class T>
    auto require(T&& x) -> valid<
        decltype(req_impl(std::forward<T>(x)))
    >;

    template<class T, class U>
    auto require(T&& x, U&& y) -> valid<
        decltype(req_impl(std::forward<T>(x))),
        decltype(req_impl(std::forward<U>(y))),
        TICK_RETURNS(x < y, bool),
        TICK_RETURNS(y < x, bool),

        TICK_RETURNS(x > y, bool),
        TICK_RETURNS(y > x, bool),

        TICK_RETURNS(x <= y, bool),
        TICK_RETURNS(y <= x, bool),

        TICK_RETURNS(x >= y, bool),
        TICK_RETURNS(y >= x, bool)
    >;
};

}

#endif
