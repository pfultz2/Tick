/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    bare.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_BARE_H
#define TICK_GUARD_BARE_H

/// bare
/// ====
/// 
/// Description
/// -----------
/// 
/// The `bare` trait remvoes both the reference and the `const`/`volatile`
/// qualifiers.
/// 
/// Synopsis
/// --------
/// 
///     template<class T>
///     struct bare;
/// 

#include <type_traits>
#include <tick/detail/using.h>

namespace tick {

template<class T>
struct bare
: std::remove_cv<typename std::remove_reference<T>::type>
{};

#if TICK_HAS_TEMPLATE_ALIAS

template<class T>
using bare_t = typename bare<T>::type;

#endif

}

#endif
