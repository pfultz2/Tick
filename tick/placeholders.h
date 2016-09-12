/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    placeholders.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_PLACEHOLDERS_H
#define TICK_GUARD_PLACEHOLDERS_H

#include <tick/integral_constant.h>

namespace tick {

template<template<class...> class F>
struct quote 
{
    template<class... Ts>
    TICK_USING(apply, F<Ts...>);
};

struct local_quote
{
    template<template<class...> class F>
    TICK_USING(quote, tick::quote<F>);
};

template<int N>
struct arg : tick::integral_constant<int, N>
{};

typedef arg<-1> _;
typedef arg<1> _1;
typedef arg<2> _2;
typedef arg<3> _3;
typedef arg<4> _4;
typedef arg<5> _5;
typedef arg<6> _6;
typedef arg<7> _7;
typedef arg<8> _8;
typedef arg<9> _9;

struct local_placeholders
{
    typedef arg<-1> _;
    typedef arg<1> _1;
    typedef arg<2> _2;
    typedef arg<3> _3;
    typedef arg<4> _4;
    typedef arg<5> _5;
    typedef arg<6> _6;
    typedef arg<7> _7;
    typedef arg<8> _8;
    typedef arg<9> _9;
};


}

#endif
