/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_ITERATOR_H
#define TICK_GUARD_IS_ITERATOR_H

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
    template<class T,class Iterator=typename bare<T>::type>
    auto require(T&&) -> valid<
        typename Iterator::difference_type,
        typename Iterator::value_type,
        typename Iterator::pointer,
        typename Iterator::reference,
        typename Iterator::iterator_category
    >;
};

template<class T>
struct has_iterator_traits<T*>
: true_type
{};

}

template<class T, class=void>
struct iterator_traits;

template<class T>
struct iterator_traits<T, TICK_CLASS_REQUIRES(detail::has_iterator_traits<T>())>
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
