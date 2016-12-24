/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_trivially_copyable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_TRIVIALLY_COPYABLE_H
#define TICK_GUARD_IS_TRIVIALLY_COPYABLE_H

/// is_trivially_copyable
/// =====================
/// 
/// Description
/// -----------
/// 
/// If a type is trivially copyable.
///

#include <tick/builder.h>

namespace tick {

// We use trival for trivally copyable since isn't implemented yet in gcc
template<class T>
struct is_trivially_copyable
: integral_constant<bool, std::is_trivial<T>::value>
{};

}

#endif
