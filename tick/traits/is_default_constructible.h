/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_default_constructible.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_DEFAULT_CONSTRUCTIBLE_H
#define TICK_GUARD_IS_DEFAULT_CONSTRUCTIBLE_H

#include <tick/builder.h>

namespace tick {

#if TICK_LEGACY_GCC

TICK_TRAIT(is_default_constructible)
{
    template<class T>
    auto require(const T&) -> valid<
        decltype(T())
    >;
};

#else

template<class T>
struct is_default_constructible
: integral_constant<bool, std::is_default_constructible<T>::value>
{};

#endif

}

#endif
