/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_nullable_pointer.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_NULLABLE_POINTER_H
#define TICK_GUARD_IS_NULLABLE_POINTER_H

/// is_nullable_pointer
/// ===================
/// 
/// Description
/// -----------
/// 
/// Checks if type `T` is a pointer-like object which can be compared to
/// `std::nullptr_t` objects.
/// 
/// Requirements
/// ------------
/// 
/// The type must meet all of the following requirements:
/// 
/// * is_equality_comparable
/// * is_default_constructible
/// * is_copy_constructible
/// * is_copy_assignable
/// * is_destructible
/// 
/// The type `T` must satisfy the following additional expressions, given two
/// values `p` and `q` that are of type `T`, and that `np` is a value of
/// `std::nullptr_t` type:
/// 
/// +------------+--------------------------------------------------------------------------------------------+
/// | Expression | Effects
/// +============+============================================================================================+
/// | `T p(np);` | Afterwards, p is equivalent to `nullptr`                                                   |
/// +------------+--------------------------------------------------------------------------------------------+
/// | `T(np)`    | a temporary object that is equivalent to `nullptr`                                         |
/// +------------+--------------------------------------------------------------------------------------------+
/// | `p = np`   | Must return a `T&`, and afterwards, `p` is equivalent to `nullptr`                         |
/// +------------+--------------------------------------------------------------------------------------------+
/// | `p != q`   | Must return a value that is contextually convertible to `bool`, The effect is `!(p == q)`  |
/// +------------+--------------------------------------------------------------------------------------------+
/// | `p == np`  | Must return a value that is contextually convertible to `bool`. The effect is `(p == T())` |
/// +------------+--------------------------------------------------------------------------------------------+
/// | `np == p`  | Must return a value that is contextually convertible to `bool`. The effect is `(p == T())` |
/// +------------+--------------------------------------------------------------------------------------------+
/// | `p != np`  | Must return a value that is contextually convertible to `bool`. The effect is `!(p == np)` |
/// +------------+--------------------------------------------------------------------------------------------+
/// | `np != p`  | Must return a value that is contextually convertible to `bool`. The effect is `!(p == np)` |
/// +------------+--------------------------------------------------------------------------------------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_nullable_pointer,
///         is_equality_comparable<_>,
///         is_default_constructible<_>,
///         is_copy_constructible<_>,
///         is_copy_assignable<_>,
///         is_destructible<_>,
///         std::is_constructible<_, std::nullptr_t>,
///         is_equality_comparable<_, std::nullptr_t>
///     )
///     {};
/// 


#include <tick/builder.h>
#include <tick/traits/is_equality_comparable.h>
#include <tick/traits/is_default_constructible.h>
#include <tick/traits/is_copy_constructible.h>
#include <tick/traits/is_copy_assignable.h>
#include <tick/traits/is_destructible.h>
#include <tick/traits/is_equality_comparable.h>

namespace tick {

TICK_TRAIT(is_nullable_pointer,
    is_equality_comparable<_>,
    is_default_constructible<_>,
    is_copy_constructible<_>,
    is_copy_assignable<_>,
    is_destructible<_>,
    std::is_constructible<_, std::nullptr_t>,
    is_equality_comparable<_, std::nullptr_t>
)
{};

}

#endif
