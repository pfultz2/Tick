/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_associative_container.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_ASSOCIATIVE_CONTAINER_H
#define TICK_GUARD_IS_ASSOCIATIVE_CONTAINER_H

/// is_associative_container
/// ========================
/// 
/// Description
/// -----------
/// 
/// An associative container is an ordered container that provides fast lookup
/// of objects based on keys.
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_associative_container, is_container<_>)
///     {
///         template<class T>
///         auto require(const T& x) -> valid<
///             has_type<typename T::key_type, is_destructible<_>>,
///             has_type<typename T::key_compare, is_compare<_, typename T::key_type>>,
///             has_type<typename T::value_compare, is_compare<_, typename T::value_type>>,
///             decltype(returns<typename T::key_compare>(x.key_comp())),
///             decltype(returns<typename T::value_compare>(x.value_comp()))
///         >;
///     };
/// 


#include <tick/builder.h>
#include <tick/traits/is_container.h>
#include <tick/traits/is_compare.h>

namespace tick {

TICK_TRAIT(is_associative_container, is_container<_>)
{
    template<class T>
    auto require(const T& x) -> valid<
        has_type<typename T::key_type, is_destructible<_>>,
        has_type<typename T::key_compare, is_compare<_, typename T::key_type>>,
        has_type<typename T::value_compare, is_compare<_, typename T::value_type>>,
        TICK_RETURNS(x.key_comp(), typename T::key_compare),
        TICK_RETURNS(x.value_comp(), typename T::value_compare)
    >;
};

}

#endif
