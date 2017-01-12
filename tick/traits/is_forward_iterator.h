/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_forward_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_FORWARD_ITERATOR_H
#define TICK_GUARD_IS_FORWARD_ITERATOR_H

/// is_forward_iterator
/// ===================
/// 
/// Description
/// -----------
/// 
/// A forward iterator is an iterator that can read data from the pointed-to
/// element.
/// 
/// Requirements
/// ------------
/// 
/// The type `It` satisfies `is_forward_iterator` if:
/// 
/// * The type `It` [`is_input_iterator`](is_input_iterator)
/// * The type `It` [`is_default_constructible`](is_default_constructible)
/// 
/// The type `std::iterator_traits<It>::reference` must be exactly
/// 
/// * `T&` if `It` satisfies OutputIterator (`It` is mutable)
/// * `const T&` otherwise (`It` is constant),
/// 
/// (where `T` is the type denoted by `std::iterator_traits<It>::value_type`)
/// 
/// And, given
/// 
/// * `i`, dereferenceable iterator of type `It`
/// * `reference`, the type denoted by `std::iterator_traits<It>::reference`
/// 
/// The following expressions must be valid and have their specified effects
/// 
/// +--------------------+--------------------+-------------------------------+
/// | Expression         | Return             | Equivalent                    |
/// |                    |                    | expression                    |
/// +====================+====================+===============================+
/// | ``i++``            | ``It``             | ``It ip=i; ++i; return ip;``  |
/// +--------------------+--------------------+-------------------------------+
/// | ``*i++``           | ``reference``      |                               |
/// +--------------------+--------------------+-------------------------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_forward_iterator, is_input_iterator<_>, is_default_constructible<_>)
///     {
///         template<class I>
///         auto require(I&& i) -> valid<
///             returns<typename std::add_lvalue_reference<I>::type>(++i),
///             returns<I>(i++),
///             is_true<std::is_reference<typename iterator_traits<I>::reference>>,
///             returns<typename iterator_traits<I>::value_type&>(*i),
///             returns<typename iterator_traits<I>::reference>(*i++)
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/traits/is_input_iterator.h>
#include <tick/traits/is_default_constructible.h>

namespace tick {

TICK_TRAIT(is_forward_iterator, is_input_iterator<_>, is_default_constructible<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(++i, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i++, I),
        is_true<std::is_reference<typename iterator_traits<I>::reference>>,
        // TICK_RETURNS(*i, typename iterator_traits<I>::value_type&),
        TICK_RETURNS(*i++, typename iterator_traits<I>::reference)
    >;
};

}

#endif
