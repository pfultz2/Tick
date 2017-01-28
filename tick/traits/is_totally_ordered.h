/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_totally_ordered.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_TOTALLY_ORDERED_H
#define TICK_GUARD_IS_TOTALLY_ORDERED_H

/// is_totally_ordered
/// ==================
/// 
/// Description
/// -----------
/// 
/// Checks if type `T` has operators for ordering and equality, and they are
/// convertible to `bool`.
/// 
/// Requirements
/// ------------
/// 
/// The type `T` satisfies `is_totally_ordered` if
/// 
/// * The type `T` satisfies [`is_weakly_ordered`](is_weakly_ordered)
/// * The type `T` satisfies [`is_equality_comparable`](is_equality_comparable)
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_totally_ordered,
///         quote<is_weakly_ordered>,
///         quote<is_equality_comparable>
///     )
///     {};
/// 

#include <tick/builder.h>
#include <tick/traits/is_weakly_ordered.h>
#include <tick/traits/is_equality_comparable.h>

namespace tick {

TICK_TRAIT(is_totally_ordered,
    quote<is_weakly_ordered>,
    quote<is_equality_comparable>
)
{};

}

#endif
