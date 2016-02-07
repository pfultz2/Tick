/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_value_swappable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_VALUE_SWAPPABLE_H
#define TICK_GUARD_IS_VALUE_SWAPPABLE_H

/// is_value_swappable
/// ==================
/// 
/// Description
/// -----------
/// 
/// Checks if `T` two objects of type `T` can be dereferenced and the resulting values satisify `is_swappable`.
/// 
/// Requirements
/// ------------
/// 
/// Type T satisfies `is_value_swappable` if
/// 
/// * Type T satisfies the requirements for `is_iterator`
/// 
/// * For any dereferencable object `x` of type `T`, `*x` satisfies the requirements for `is_swappable`
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_value_swappable, is_iterator<_>)
///     {
///         template<class T>
///         auto require(T&&) -> valid<
///             is_true<is_swappable<typename iterator_traits<T>::value_type>>
///         >;
///     };
/// 


#include <tick/builder.h>
#include <tick/traits/is_iterator.h>
#include <tick/traits/is_swappable.h>

namespace tick {

TICK_TRAIT(is_value_swappable, is_iterator<_>)
{
    template<class T>
    auto require(T&&) -> valid<
        is_true<is_swappable<typename iterator_traits<T>::value_type>>
    >;
};

}

#endif
