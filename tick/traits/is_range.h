/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_range.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_RANGE_H
#define TICK_GUARD_IS_RANGE_H

/// is_range
/// ========
/// 
/// Description
/// -----------
/// 
/// Checks if the type provides begin and end iterators that can be used with
/// a for-range loop.
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_range)
///     {
///         using std::begin;
///         using std::end;
///         template<class T>
///         auto require(T&& x) -> valid<
///             decltype(returns<is_iterator<_>>(begin(std::forward<T>(x)))),
///             decltype(returns<is_iterator<_>>(end(std::forward<T>(x))))
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/traits/is_iterator.h>

namespace tick_adl {

using std::begin;
using std::end;

template<class T>
auto begin(T&& x) -> decltype(begin(std::forward<T>(x)));

template<class T>
auto end(T&& x) -> decltype(end(std::forward<T>(x)));

}

namespace tick {

TICK_TRAIT(is_range)
{
    template<class T>
    auto require(T&& x) -> valid<
        TICK_RETURNS(tick_adl::begin(std::forward<T>(x)), is_iterator<_>),
        TICK_RETURNS(tick_adl::end(std::forward<T>(x)), is_iterator<_>)
    >;
};

}

#endif
