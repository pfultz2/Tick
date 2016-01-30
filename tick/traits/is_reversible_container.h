/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_reversible_container.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_REVERSIBLE_CONTAINER_H
#define TICK_GUARD_IS_REVERSIBLE_CONTAINER_H

#include <tick/builder.h>
#include <tick/traits/is_container.h>

namespace tick {

TICK_TRAIT(is_reversible_container, is_container<_>)
{
    template<class T>
    auto require(const T& x) -> valid<
        TICK_HAS_TYPE(T::reverse_iterator, is_iterator<_>),
        TICK_HAS_TYPE(T::const_reverse_iterator, is_iterator<_>),
        TICK_RETURNS(x.crbegin(), typename T::const_reverse_iterator),
        TICK_RETURNS(x.crend(), typename T::const_reverse_iterator),
        TICK_RETURNS(x.rbegin(), typename T::const_reverse_iterator),
        TICK_RETURNS(x.rend(), typename T::const_reverse_iterator),
        TICK_RETURNS(as_mutable(x).rbegin(), typename T::reverse_iterator),
        TICK_RETURNS(as_mutable(x).rend(), typename T::reverse_iterator)
    >;
};

}

#endif
