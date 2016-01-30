/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_destructible.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_DESTRUCTIBLE_H
#define TICK_GUARD_IS_DESTRUCTIBLE_H

/// is_destructible
/// ==================
/// 
/// Description
/// -----------
/// 
/// Checks if type `T` can be destructed.
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_destructible)
///     {
///         template<class T>
///         auto require(const T& x) -> valid<
///             decltype(as_mutable(x).~T())
///         >;
///     };
/// 

#include <tick/builder.h>

namespace tick {

#if TICK_LEGACY_GCC

namespace detail {

TICK_TRAIT(is_destructible_impl)
{
    template<class T>
    auto require(const T&) -> valid<
        decltype(std::declval<T&>().~T())
    >;
};
}

template<class T>
struct is_destructible
: std::conditional<(std::is_lvalue_reference<T>::value or std::is_rvalue_reference<T>::value),
    true_type,
    detail::is_destructible_impl<typename std::remove_all_extents<T>::type>
>::type
{};

template<>
struct is_destructible<void>
: false_type
{};

#else

template<class T>
struct is_destructible
: integral_constant<bool, std::is_destructible<T>::value>
{};

#endif

}

#endif
