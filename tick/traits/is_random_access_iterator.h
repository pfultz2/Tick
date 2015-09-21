/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_random_access_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_RANDOM_ACCESS_ITERATOR_H
#define TICK_GUARD_IS_RANDOM_ACCESS_ITERATOR_H

#include <tick/builder.h>
#include <tick/traits/is_bidirectional_iterator.h>
#include <tick/traits/is_totally_ordered.h>

namespace tick {

TICK_TRAIT(is_random_access_iterator, is_bidirectional_iterator<_>, is_totally_ordered<_>)
{
    template<class I, class Number>
    static auto req_impl(I&& i, Number n) -> valid<
        TICK_RETURNS(i += n, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i -= n, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i + n, I),
        TICK_RETURNS(i - n, I),
        TICK_RETURNS(n + i, I),
        TICK_RETURNS(i - i, typename iterator_traits<I>::difference_type),
        TICK_RETURNS(i[n], typename iterator_traits<I>::reference),
        TICK_RETURNS(*(i + n), typename iterator_traits<I>::reference)
    >;

    template<class I, class Number>
    auto require(I&& i, Number n) -> valid<
        decltype(req_impl(std::forward<I>(i), n))
    >;

    template<class I>
    auto require(I&& i) -> valid<
        decltype(req_impl(std::forward<I>(i), 0))
    >;
};

}

#endif
