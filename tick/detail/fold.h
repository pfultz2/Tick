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
    using result = typename fold_impl<sizeof...(Xs) == 0>::template result<F, typename F<State, X>::type, Xs...>;
};

template <> struct fold_impl<true> 
{
    template <template <class ...> class F, class State, class ...>
    using result = State;
};

template <class List, class State, template <class ...> class F>
struct fold;

template <template <class ...> class List, class State, template <class ...> class F, class ...Xs>
struct fold<List<Xs...>, State, F>
{
    using type = typename fold_impl<sizeof...(Xs) == 0>::
                 template result<F, State, Xs...>;
};

}}

#endif