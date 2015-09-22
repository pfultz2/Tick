/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_allocator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_ALLOCATOR_H
#define TICK_GUARD_IS_ALLOCATOR_H

#include <tick/builder.h>
#include <tick/traits/is_nullable_pointer.h>
#include <tick/traits/is_random_access_iterator.h>

namespace tick {

TICK_TRAIT(is_allocator,
    is_copy_constructible<_>,
    is_equality_comparable<_>
)
{
    // template<class A, class T>
    // auto require(const A& a, const T&) -> valid<
    //     TICK_HAS_TYPE(A::value_type, T),
    //     TICK_RETURNS(a.allocate(std::declval<std::size_t>()), is_nullable_pointer<_>, is_random_access_iterator<_>),
    //     TICK_RETURNS(*a.allocate(std::declval<std::size_t>()), typename A::value_type&)
    // >;

    template<class A>
    auto require(const A& a)
    { 
        return valid<
            TICK_RETURNS(as_mutable(a).allocate(std::declval<std::size_t>()), is_nullable_pointer<_>, is_random_access_iterator<_>),
            TICK_RETURNS(*(as_mutable(a).allocate(std::declval<std::size_t>())), typename A::value_type&)
        >();
    }
};

}

#endif
