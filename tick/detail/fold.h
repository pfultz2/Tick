/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    fold.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_FOLD_H
#define TICK_GUARD_FOLD_H

namespace tick { namespace detail {

template <bool done> struct fold_impl 
{
    template <template <class ...> class F, class State, class X, class ...Xs>
    struct apply
    {
        typedef typename fold_impl<sizeof...(Xs) == 0>::template apply<F, typename F<State, X>::type, Xs...>::type type;
    };
};

template <> struct fold_impl<true> 
{
    template <template <class ...> class F, class State, class ...>
    struct apply
    {
        typedef State type;
    };
};

template <class List, class State, template <class ...> class F>
struct fold;

template <template <class ...> class List, class State, template <class ...> class F, class ...Xs>
struct fold<List<Xs...>, State, F>
{
    typedef typename fold_impl<sizeof...(Xs) == 0>::template apply<F, State, Xs...>::type type;
};

}}

#endif
