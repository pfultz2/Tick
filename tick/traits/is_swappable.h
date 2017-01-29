/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_swappable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_SWAPPABLE_H
#define TICK_GUARD_IS_SWAPPABLE_H

/// is_swappable
/// ============
/// 
/// Description
/// -----------
/// 
/// Checks if type `T` can be called with either `std::swap` or an ADL
/// overload of `swap`.
/// 
/// Requirements
/// ------------
/// 
/// Type `U` is swappable with type `T` if, for any object `u` of type `U` and
/// any object `t` of type `T`
/// 
/// 
/// +--------------------------------+------------------------------------------+
/// | Expression                     | Requirements                             |
/// +================================+==========================================+
/// | `using std::swap; swap(t, u);` | After the call, the value of t is the    |
/// |                                | value held by u before the call, and the |
/// |                                | value of u is the value held by t before |
/// |                                | the call.                                |
/// +--------------------------------+------------------------------------------+
/// | `using std::swap; swap(u, t);` | After the call, the value of t is the    |
/// |                                | value held by u before the call, and the |
/// |                                | value of u is the value held by t before |
/// |                                | the call.                                |
/// +--------------------------------+------------------------------------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_swappable)
///     {
///         using std::swap;
///         template<class T>
///         auto require(T&& x) -> valid<
///             decltype(swap(x, x))
///         >;
///     
///         template<class T, class U>
///         auto require(T&& x, U&& y) -> valid<
///             decltype(swap(x, y))
///             decltype(swap(y, x))
///         >;
///     };
/// 



#include <tick/builder.h>

namespace tick_adl {

using std::swap;

template<class T, class U>
auto swap(T&& x, U&& y) -> decltype(swap(std::forward<T>(x), std::forward<U>(y)));

}

namespace tick {

TICK_TRAIT(is_swappable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(tick_adl::swap(x, x))
    >;

    template<class T, class U>
    auto require(T&& x, U&& y) -> valid<
        decltype(tick_adl::swap(x, y)),
        decltype(tick_adl::swap(y, x))
    >;
};

}

#endif
