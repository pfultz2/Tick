/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_sequence_container.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_SEQUENCE_CONTAINER_H
#define TICK_GUARD_IS_SEQUENCE_CONTAINER_H

#include <tick/builder.h>
#include <tick/traits/is_container.h>

namespace tick {

TICK_TRAIT(is_sequence_container, is_container<_>)
{
    template<class X, class T, class N, class Iterator>
    static auto req_impl(X& a, const T& t, N n, const Iterator& p) -> valid<
        TICK_RETURNS(a.insert(p, t), Iterator),
        // This should return an iterator too in C++11 but currently gcc does not
        // implement it
        TICK_RETURNS(a.insert(p, n, t), void),
        TICK_RETURNS(a.erase(p), Iterator),
        TICK_RETURNS(a.erase(p, p), Iterator),
        decltype(a.clear()),
        decltype(a.assign(n, t))
    >;

    template<class T>
    auto require(const T& x) -> valid<
        decltype(req_impl(
            as_mutable(x), 
            std::declval<typename T::value_type>(), 
            std::declval<typename T::size_type>(),
            // Should be a const_iterator, but many containers don't implement
            // this correctly
            std::declval<typename T::iterator>()
        ))
    >;
};

}

#endif
