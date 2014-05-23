/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    placeholders.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_PLACEHOLDERS_H
#define TICK_GUARD_PLACEHOLDERS_H

#include <type_traits>

namespace tick {

template<int N>
using arg = std::integral_constant<int, N>;

using _ = arg<-1>;
using _1 = arg<1>;
using _2 = arg<2>;
using _3 = arg<3>;
using _4 = arg<4>;
using _5 = arg<5>;
using _6 = arg<6>;
using _7 = arg<7>;
using _8 = arg<8>;
using _9 = arg<9>;

struct local_placeholders
{
    using _ = arg<-1>;
    using _1 = arg<1>;
    using _2 = arg<2>;
    using _3 = arg<3>;
    using _4 = arg<4>;
    using _5 = arg<5>;
    using _6 = arg<6>;
    using _7 = arg<7>;
    using _8 = arg<8>;
    using _9 = arg<9>;
};


}

#endif