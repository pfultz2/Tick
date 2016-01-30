/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_totally_ordered.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_TOTALLY_ORDERED_H
#define TICK_GUARD_IS_TOTALLY_ORDERED_H

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
