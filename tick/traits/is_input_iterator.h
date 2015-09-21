/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_input_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_INPUT_ITERATOR_H
#define TICK_GUARD_IS_INPUT_ITERATOR_H

#include <tick/builder.h>
#include <tick/traits/is_iterator.h>
#include <tick/traits/is_equality_comparable.h>

namespace tick {

TICK_TRAIT(is_input_iterator, is_iterator<_>, is_equality_comparable<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(*i, typename iterator_traits<I>::value_type),
        TICK_RETURNS(*i++, typename iterator_traits<I>::value_type)
    >;
};

}

#endif
