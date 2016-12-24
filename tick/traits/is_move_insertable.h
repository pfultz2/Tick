/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_move_insertable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_MOVE_INSERTABLE_H
#define TICK_GUARD_IS_MOVE_INSERTABLE_H

/// is_move_insertable
/// ==================
/// 
/// Description
/// -----------
/// 
/// If a type can be constructed into uninitialized storage from an rvalue of
/// that type by a given allocator.
/// 
/// Synopsis
/// --------
/// 
///     template<class C, class T>
///     struct is_move_insertable
///     : is_emplace_constructible<C, T, T&&>
///     {};
/// 


#include <tick/builder.h>
#include <tick/traits/is_emplace_constructible.h>
#include <memory>

namespace tick {

template<class C, class T>
struct is_move_insertable
: is_emplace_constructible<C, T, T&&>
{};

}

#endif
