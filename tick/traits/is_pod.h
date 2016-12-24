/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_pod.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_POD_H
#define TICK_GUARD_IS_POD_H

/// is_pod
/// ======
/// 
/// Description
/// -----------
/// 
/// If a type is a POD (Plain Old Data) type. This means the type is
/// compatible with the types used in the C programming language, can be
/// manipulated using C library functions: it can be created with std::malloc,
/// it can be copied with std::memmove, etc, and can be exchanged with C
/// libraries directly, in its binary form.
/// 

#include <tick/builder.h>

namespace tick {

template<class T>
struct is_pod
: integral_constant<bool, std::is_pod<T>::value>
{};

}

#endif
