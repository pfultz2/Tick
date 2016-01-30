/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_forward_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_FORWARD_ITERATOR_H
#define TICK_GUARD_IS_FORWARD_ITERATOR_H

#include <tick/builder.h>
#include <tick/traits/is_input_iterator.h>
#include <tick/traits/is_default_constructible.h>

namespace tick {

TICK_TRAIT(is_forward_iterator, is_input_iterator<_>, is_default_constructible<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(++i, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i++, I),
        TICK_IS_TRUE(std::is_reference<typename iterator_traits<I>::reference>),
        // TICK_RETURNS(*i, typename iterator_traits<I>::value_type&),
        TICK_RETURNS(*i++, typename iterator_traits<I>::reference)
    >;
};

}

#endif
