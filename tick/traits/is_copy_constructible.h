/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_copy_constructible.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_COPY_CONSTRUCTIBLE_H
#define TICK_GUARD_IS_COPY_CONSTRUCTIBLE_H

#include <tick/builder.h>

namespace tick {

#if TICK_LEGACY_GCC

TICK_TRAIT(is_copy_constructible)
{
    template<class T>
    auto require(const T& x) -> valid<
        decltype(T(x))
    >;
};

#else

template<class T>
struct is_copy_constructible
: integral_constant<bool, std::is_copy_constructible<T>::value>
{};

#endif

}

#endif
