/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_value_swappable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_VALUE_SWAPPABLE_H
#define TICK_GUARD_IS_VALUE_SWAPPABLE_H

#include <tick/builder.h>
#include <tick/traits/is_iterator.h>
#include <tick/traits/is_swappable.h>

namespace tick {

TICK_TRAIT(is_value_swappable, is_iterator<_>)
{
    template<class T>
    auto require(T&&) -> valid<
        TICK_IS_TRUE(is_swappable<typename iterator_traits<T>::value_type>)
    >;
};

}

#endif
