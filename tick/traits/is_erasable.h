/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_erasable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_ERASABLE_H
#define TICK_GUARD_IS_ERASABLE_H

#include <tick/builder.h>
#include <tick/traits/is_allocator.h>
#include <memory>

namespace tick {

TICK_TRAIT(is_erasable)
{
    template<class C, class T>
    auto require(const C&, const T&) -> valid<
        has_type<typename C::allocator_type, is_allocator<_>>,
        TICK_RETURNS(C::get_allocator(), typename C::allocator_type&),
        decltype(
// Gcc 4.6 doesn't have allocator traits
#if defined (__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7
            void()
#else
            std::allocator_traits<typename C::allocator_type>::destroy(C::get_allocator(), std::declval<T*>())
#endif
        )
    >;
};

}

#endif
