/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_container.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_CONTAINER_H
#define TICK_GUARD_IS_CONTAINER_H

#include <tick/builder.h>
#include <tick/traits/is_equality_comparable.h>
#include <tick/traits/is_default_constructible.h>
#include <tick/traits/is_copy_constructible.h>
#include <tick/traits/is_copy_assignable.h>
#include <tick/traits/is_destructible.h>
#include <tick/traits/is_swappable.h>
#include <tick/traits/is_range.h>

namespace tick {

// TODO: Add CopyInsertable
TICK_TRAIT(is_container,
    is_equality_comparable<_>,
    is_default_constructible<_>,
    is_copy_constructible<_>,
    is_copy_assignable<_>,
    is_destructible<_>,
    is_swappable<_>,
    is_range<_>
)
{
    template<class T>
    auto require(const T& x) -> valid<
        TICK_HAS_TYPE(T::iterator, is_iterator<_>),
        TICK_HAS_TYPE(T::const_iterator, is_iterator<_>),
        TICK_HAS_TYPE(T::value_type),
        TICK_HAS_TYPE(T::reference, typename T::value_type&),
        TICK_HAS_TYPE(T::const_reference, const typename T::value_type&),
        TICK_HAS_TYPE(T::difference_type, int),
        TICK_HAS_TYPE(T::size_type, unsigned),
        TICK_RETURNS(x.size(), typename T::size_type),
        TICK_RETURNS(x.max_size(), typename T::size_type),
        TICK_RETURNS(x.empty(), bool)
    >;
};

}

#endif
