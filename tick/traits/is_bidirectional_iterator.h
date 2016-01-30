/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_bidirectional_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_BIDIRECTIONAL_ITERATOR_H
#define TICK_GUARD_IS_BIDIRECTIONAL_ITERATOR_H

#include <tick/builder.h>
#include <tick/traits/is_forward_iterator.h>

namespace tick {

TICK_TRAIT(is_bidirectional_iterator, is_forward_iterator<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(--i, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i--, I),
        TICK_RETURNS(*i--, typename iterator_traits<I>::reference)
    >;
};

}

#endif
