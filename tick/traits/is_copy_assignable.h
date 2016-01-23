/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_copy_assignable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_COPY_ASSIGNABLE_H
#define TICK_GUARD_IS_COPY_ASSIGNABLE_H

/// is_copy_assignable
/// ==================
/// 
/// Description
/// -----------
/// 
/// Checks if type `T` can be copy-assigned from an lvalue.
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_copy_assignable)
///     {
///         template<class T>
///         auto require(T&& x) -> valid<
///             decltype(x = as_const(x))
///         >;
///     };
/// 

#include <tick/builder.h>

namespace tick {

#if TICK_LEGACY_GCC

TICK_TRAIT(is_copy_assignable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x = as_const(x))
    >;
};

#else

template<class T>
struct is_copy_assignable
: integral_constant<bool, std::is_copy_assignable<T>::value>
{};

#endif

}

#endif
