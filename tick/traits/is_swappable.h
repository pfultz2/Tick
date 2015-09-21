/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_swappable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_SWAPPABLE_H
#define TICK_GUARD_IS_SWAPPABLE_H

#include <tick/builder.h>

namespace tick_adl {

using std::swap;

template<class T, class U>
auto swap(T&& x, U&& y) -> decltype(swap(std::forward<T>(x), std::forward<U>(y)));

}

namespace tick {

TICK_TRAIT(is_swappable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(tick_adl::swap(x, x))
    >;
};

}

#endif
