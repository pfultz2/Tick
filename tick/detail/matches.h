/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    matches.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_DETAIL_GUARD_MATCHES_H
#define TICK_DETAIL_GUARD_MATCHES_H

#include <tick/placeholders.h>
#include <tick/detail/using.h>
#include <tuple>

namespace tick { namespace detail {

template<class Tuple>
struct tuple_replace;

template<class... Ts>
struct tuple_replace<std::tuple<Ts...>>
{
    typedef std::tuple<Ts...> Tuple;
    template<class Placeholder>
    struct apply
    : std::tuple_element<Placeholder::value - 1, Tuple>
    {
        static_assert(static_cast<std::size_t>(Placeholder::value) <= std::tuple_size<Tuple>::value, "Invalid placeholder");
    };

    template<template<class...> class Template>
    struct quote
    {
        typedef Template<Ts...> type;
    };
};

template<class T>
struct single_replace
{
    template<class Placeholder>
    struct apply
    {
        static_assert(Placeholder::value == 1, "Invalid placeholder");
        typedef T type;
    };

    template<template<class...> class Template>
    struct quote
    {
        typedef Template<T> type;
    };
};

template<class F, class Replacer>
struct replace_args_recursive
{
    typedef F type;
};

template<int N, class Replacer>
struct replace_args_recursive<arg<N>, Replacer>
: Replacer::template apply<std::integral_constant<int, N>>
{};
// TODO: Add some pattern matching for integer parameters
template<template<class...> class F, class Replacer, class... Args>
struct replace_args_recursive<F<Args...>, Replacer>
{
    typedef F<typename replace_args_recursive<Args, Replacer>::type...> type;
};

template<template<class...> class F, class Replacer>
struct replace_args_recursive<quote<F>, Replacer>
{
    typedef typename Replacer::template quote<F>::type type;
};
#if !TICK_HAS_TEMPLATE_ALIAS
template<template<class...> class F, class Replacer>
struct replace_args_recursive<local_quote::quote<F>, Replacer>
{
    typedef typename Replacer::template quote<F>::type type;
};
#endif

template<class F, class Replacer>
struct replace_args_impl
: replace_args_recursive<F, Replacer>
{};

// Specialization for simple unnamed placeholders
template<template<class...> class F, class Replacer, class... Args>
struct replace_args_impl<F<arg<-1>, Args...>, Replacer>
{
    typedef F<
        typename Replacer::template apply<std::integral_constant<int, 1>>::type, 
        typename replace_args_recursive<Args, Replacer>::type...
    > type;
};

template<template<class...> class F, class Replacer, class Arg, class... Args>
struct replace_args_impl<F<Arg, arg<-1>, Args...>, Replacer>
{
    typedef F<
        typename replace_args_recursive<Arg, Replacer>::type,
        typename Replacer::template apply<std::integral_constant<int, 1>>::type, 
        typename replace_args_recursive<Args, Replacer>::type...
    > type;
};

template<template<class...> class F, class Replacer, class... Args>
struct replace_args_impl<F<arg<-1>, arg<-1>, Args...>, Replacer>
{
    typedef F<
        typename Replacer::template apply<std::integral_constant<int, 1>>::type, 
        typename Replacer::template apply<std::integral_constant<int, 2>>::type, 
        typename replace_args_recursive<Args, Replacer>::type...
    > type;
};

template<class F, class... Ts>
struct replace_args
: detail::replace_args_impl<F, detail::tuple_replace<std::tuple<Ts...>>>
{};

// Specializations for the most common use cases
template<template<class...> class F, class T, class... Args>
struct replace_args<F<Args...>, T>
: detail::replace_args_impl<F<Args...>, detail::single_replace<T>>
{};

template<class T, class U, class R>
struct match_impl
: R
{};

template<class T, class U>
struct match_impl<T, U, U>
: std::is_convertible<T, U>
{};

template<class T>
struct match_impl<T, void, void>
: std::true_type
{};

template<class T, class U>
TICK_USING(matches, match_impl<T, U, typename replace_args<U, T>::type>);

}}

#endif
