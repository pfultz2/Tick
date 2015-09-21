/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_move_constructible.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_MOVE_CONSTRUCTIBLE_H
#define TICK_GUARD_IS_MOVE_CONSTRUCTIBLE_H

#include <tick/builder.h>

namespace tick {

#if TICK_LEGACY_GCC

TICK_TRAIT(is_move_constructible)
{
    template<class T>
    auto require(const T& x) -> valid<
        decltype(T(std::move(x)))
    >;
};

#else

template<class T>
struct is_move_constructible
: integral_constant<bool, std::is_move_constructible<T>::value>
{};

#endif

}

#endif
