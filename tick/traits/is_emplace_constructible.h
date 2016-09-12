/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_emplace_constructible.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_EMPLACE_CONSTRUCTIBLE_H
#define TICK_GUARD_IS_EMPLACE_CONSTRUCTIBLE_H

#include <tick/builder.h>
#include <tick/traits/is_allocator.h>
#include <memory>

namespace tick {

TICK_TRAIT(is_emplace_constructible)
{
    template<class C, class T, class... Ts>
    auto require(const C& c, const T&, Ts&&...) -> valid<
        has_type<typename C::allocator_type, is_allocator<_>>,
        TICK_RETURNS(c.get_allocator(), typename C::allocator_type),
        decltype(
// Allocator traits construct is not supported on gcc 4.7 and earlier
#if defined (__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 8
            void()
#else
            std::allocator_traits<typename C::allocator_type>::construct(as_mutable(c.get_allocator()), std::declval<T*>(), std::declval<Ts>()...)
#endif
        )
    >;
};

}

#endif
