/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_output_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_OUTPUT_ITERATOR_H
#define TICK_GUARD_IS_OUTPUT_ITERATOR_H

#include <tick/builder.h>
#include <tick/traits/is_iterator.h>

namespace tick {

TICK_TRAIT(is_output_iterator, is_iterator<_>)
{
    template<class I, class T>
    static auto req_impl(I&& i, T&& x) -> valid<
        decltype(*i = x),
        decltype(*i++ = x),
        TICK_RETURNS(++i, typename std::add_const<typename std::add_lvalue_reference<I>::type>::type)
    >;

    template<class I, class T>
    auto require(I&& i, T&& x) -> valid<
        decltype(req_impl(std::forward<I>(i), std::forward<T>(x)))
    >;

    template<class I>
    auto require(I&& i) -> valid<
        decltype(req_impl(std::forward<I>(i), std::declval<typename iterator_traits<I>::value_type>()))
    >;
};

}

#endif
