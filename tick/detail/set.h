/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    set.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_SET_H
#define TICK_GUARD_SET_H

#include <type_traits>

namespace tick { namespace detail {

template<class T>
struct type {};

template<class... Ts>
struct set : type<Ts>...
{};

template<class Set, class T>
struct set_contains
: std::is_base_of<type<T>, Set>
{};

template<class Set, class T>
struct set_insert;

template<template<class... Ts> class Set, class T, class... Ts>
struct set_insert<Set<Ts...>, T>
: std::conditional<set_contains<Set<Ts...>, T>::value,
    Set<Ts...>,
    Set<Ts..., T>
>
{};

}}

#endif
