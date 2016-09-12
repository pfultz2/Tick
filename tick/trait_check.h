/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    trait_check.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_TRAIT_CHECK_H
#define TICK_GUARD_TRAIT_CHECK_H

#include <tick/builder.h>
#include <tick/detail/fold.h>
#include <tick/detail/pp.h>
#include <tick/detail/set.h>

namespace tick {

template<class... T>
struct TRAIT_CHECK_FAILURE;

template<>
struct TRAIT_CHECK_FAILURE<>
{};

namespace detail {  

template<int... N>
struct trait_check_test
{};

template<class T, class TraitCheck>
struct trait_check_push_back;

template<class T, class... Ts>
struct trait_check_push_back<T, TRAIT_CHECK_FAILURE<Ts...>>
{
    typedef TRAIT_CHECK_FAILURE<T, Ts...> type;
};

template<class T>
struct box
{
    typedef T type;
};

template<class TraitCheck, class T>
struct trait_check_fold_each
: std::conditional<T::value,
    box<TraitCheck>, 
    trait_check_push_back<T, TraitCheck>
>::type
{};

template<class Set>
struct trait_check_fold
: fold<Set, 
    TRAIT_CHECK_FAILURE<>, 
    trait_check_fold_each
>
{};

template<class Set, class T>
struct trait_check_insert_trait
: trait_check_insert_trait<typename set_insert<Set, T>::type, typename base_traits_type<T>::type>
{};

template<class Set, class... Ts>
struct trait_check_insert_trait<Set, base_traits<Ts...>>
: fold<
    base_traits<Ts...>,
    Set,
    trait_check_insert_trait
>
{};

template<class Set, class... Ts>
struct trait_check_insert_traits
: fold<
    base_traits<Ts...>,
    Set,
    trait_check_insert_trait
>
{};

template<class... Ts>
struct trait_check_impl
: trait_check_fold<typename trait_check_insert_traits<set<>, Ts...>::type>
{};

}

template<class... Ts>
struct trait_check
{
    typedef detail::trait_check_test<sizeof(typename detail::trait_check_impl<Ts...>::type)> type;
};


}

#define TICK_TRAIT_CHECK(...) typedef typename tick::trait_check<__VA_ARGS__>::type TICK_PP_CAT(tick_trait_check_typedef_, __LINE__)



#endif
