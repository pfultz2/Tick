/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_trivial.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_TRIVIAL_H
#define TICK_GUARD_IS_TRIVIAL_H

#include <tick/builder.h>

namespace tick {

template<class T>
struct is_trivial
: integral_constant<bool, std::is_trivial<T>::value>
{};

}

#endif
