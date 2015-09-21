/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_less_than_comparable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_LESS_THAN_COMPARABLE_H
#define TICK_GUARD_IS_LESS_THAN_COMPARABLE_H

#include <tick/builder.h>

namespace tick {

TICK_TRAIT(is_less_than_comparable)
{
    template<class T>
    auto require(T&& x) -> valid<
        TICK_RETURNS(x < x, bool)
    >;

    template<class T, class U>
    auto require(T&& x, U&& y) -> valid<
        TICK_RETURNS(x < x, bool),
        TICK_RETURNS(y < y, bool),
        TICK_RETURNS(x < y, bool),
        TICK_RETURNS(y < x, bool)
    >;
};

}

#endif
