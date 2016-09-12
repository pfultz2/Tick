/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_copy_insertable.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_COPY_INSERTABLE_H
#define TICK_GUARD_IS_COPY_INSERTABLE_H

#include <tick/builder.h>
#include <tick/traits/is_move_insertable.h>

namespace tick {

TICK_TRAIT(is_copy_insertable,
    is_move_insertable<_1, _2>
)
{
    template<class C, class T>
    auto require(const C&, const T&) -> valid<
        is_true<is_emplace_constructible<C, T, T>>
    >;
};

}

#endif
