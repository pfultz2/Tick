/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_predicate.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_PREDICATE_H
#define TICK_GUARD_IS_PREDICATE_H

/// is_predicate
/// ============
/// 
/// Description
/// -----------
/// 
/// A predicate is a function object that returns a boolean.
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_predicate)
///     {
///         template<class F, class... Ts>
///         auto require(F&& f, Ts&&... xs) -> valid<
///             decltype(returns<bool>(f(std::forward<Ts>(xs)...)))
///         >;
///     };
/// 

#include <tick/builder.h>

namespace tick {

TICK_TRAIT(is_predicate)
{
    template<class F, class... Ts>
    auto require(F&& f, Ts&&... xs) -> valid<
        TICK_RETURNS(f(std::forward<Ts>(xs)...), bool)
    >;
};

}

#endif
