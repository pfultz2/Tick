/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_ITERATOR_H
#define TICK_GUARD_IS_ITERATOR_H

/// is_iterator
/// ===========
/// 
/// Description
/// -----------
/// 
/// An iterator can be used to identify and traverse the elements of a
/// container.
/// 
/// Requirements
/// ------------
/// 
/// The type `It` satisfies `is_iterator` if
/// 
/// * The type `It` satisfies [`is_copy_constructible`](is_copy_constructible)
/// * The type `It` satisfies [`is_copy_assignable`](is_copy_constructible)
/// * The type `It` satisfies [`is_destructible`](is_destructible)
/// * lvalues of type `It` satisfy [`is_swappable`](is_destructible)
/// 
/// and given:
/// 
/// * `r`, an lvalue of type `It`.
/// 
/// The following expressions must be valid and have their specified
/// effects:
/// 
/// +--------------+---------------+----------------------------------------------------------------------------+
/// | Expression   | Return Type   | Precondition                                                               |
/// +==============+===============+============================================================================+
/// | `*r`         | unspecified   | `r` is *dereferenceable*                                                   |
/// +--------------+---------------+----------------------------------------------------------------------------+
/// | `++r`        | `It&`         | `r` is *incrementable* (the behavior of the expression ++r is defined)     |
/// +--------------+---------------+----------------------------------------------------------------------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_iterator,
///         is_copy_constructible<_>,
///         is_copy_assignable<_>,
///         is_destructible<_>,
///         is_swappable<_>
///     )
///     {
///         template<class I>
///         auto require(I&& i) -> valid<
///             decltype(returns<typename std::iterator_traits<I>::reference>(*i))
///             decltype(returns<typename std::add_lvalue_reference<I>::type>(++i))
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/requires.h>
#include <tick/traits/bare.h>
#include <tick/traits/is_copy_constructible.h>
#include <tick/traits/is_copy_assignable.h>
#include <tick/traits/is_destructible.h>
#include <tick/traits/is_swappable.h>
#include <iterator>

namespace tick {

namespace detail {

// A trait to detect iterator traits, since it is not SFINAE friendly
TICK_TRAIT(has_iterator_traits)
{
    template<class Iterator>
    auto require(const Iterator&) -> valid<
        typename Iterator::difference_type,
        typename Iterator::value_type,
        typename Iterator::pointer,
        typename Iterator::reference,
        typename Iterator::iterator_category
    >;

    // template<class T>
    // auto require(T*x) -> valid<
    //     decltype(*x)
    // >;
};

}

template<class T, class=void>
struct iterator_traits;

template<class T>
struct iterator_traits<T, TICK_CLASS_REQUIRES(
    std::is_pointer<typename bare<T>::type>() or
    detail::has_iterator_traits<T>() 
)>
: std::iterator_traits<typename bare<T>::type>
{};

TICK_TRAIT(is_iterator,
    is_copy_constructible<_>,
    is_copy_assignable<_>,
    is_destructible<_>,
    is_swappable<_>
)
{
    template<class I>
    auto require(I&& i) -> valid<
        // TICK_RETURNS(*i, std::is_same<_, typename iterator_traits<I>::reference>),
        TICK_RETURNS(*i, typename iterator_traits<I>::reference),
        TICK_RETURNS(++i, typename std::add_lvalue_reference<I>::type)
    >;
};

}

#endif
