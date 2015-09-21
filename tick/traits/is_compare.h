/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_compare.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_COMPARE_H
#define TICK_GUARD_IS_COMPARE_H

#include <tick/builder.h>

namespace tick {

TICK_TRAIT(is_compare)
{
    template<class F, class T>
    static auto req_impl(F&& f, T&& x) -> valid<
        TICK_RETURNS(f(std::forward<T>(x), std::forward<T>(x)), bool)
    >;

    template<class F, class T>
    auto require(F&& f, T&& x) -> valid<
        decltype(req_impl(std::forward<F>(f), std::forward<T>(x)))
    >;

    template<class F, class T, class U>
    auto require(F&& f, T&& x, U&& y) -> valid<
        decltype(req_impl(std::forward<F>(f), std::forward<T>(x))),
        decltype(req_impl(std::forward<F>(f), std::forward<U>(y))),
        TICK_RETURNS(f(std::forward<T>(x), std::forward<U>(y)), bool),
        TICK_RETURNS(f(std::forward<U>(y), std::forward<T>(x)), bool)
    >;
};

}

#endif
