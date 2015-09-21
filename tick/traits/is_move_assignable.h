/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_move_assignable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_MOVE_ASSIGNABLE_H
#define TICK_GUARD_IS_MOVE_ASSIGNABLE_H

#include <tick/builder.h>

namespace tick {

#if TICK_LEGACY_GCC

TICK_TRAIT(is_move_assignable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x = std::move(x))
    >;
};

#else

template<class T>
struct is_move_assignable
: integral_constant<bool, std::is_move_assignable<T>::value>
{};

#endif

}

#endif
