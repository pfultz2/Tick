/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_standard_layout.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_STANDARD_LAYOUT_H
#define TICK_GUARD_IS_STANDARD_LAYOUT_H

#include <tick/builder.h>

namespace tick {

template<class T>
struct is_standard_layout
: integral_constant<bool, std::is_standard_layout<T>::value>
{};

}

#endif
