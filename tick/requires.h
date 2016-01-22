/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    requires.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_REQUIRES_H
#define TICK_GUARD_REQUIRES_H

#include <type_traits>
#include <tick/integral_constant.h>

namespace tick { 

namespace detail {

template<class T>
struct param_extract
{
    static_assert(!std::is_integral<T>::value, "TICK_PARAM_REQUIRES requires dependent typing");
    static const bool value = T::value;
};

template<long N>
struct private_enum
{
    enum class type
    {
        na
    };
};

template<template<class...> class Trait, class... Ts>
struct as_integral_constant
{
    typedef Trait<typename std::remove_reference<Ts>::type...> result_type;
    typedef integral_constant<typename result_type::value_type, result_type::value> type;
};

}

template<template<class...> class Trait, class... Ts>
constexpr typename detail::as_integral_constant<Trait, Ts...>::type 
trait(Ts&&...) noexcept
{
    return typename detail::as_integral_constant<Trait, Ts...>::type();
}
}

#define TICK_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE(...) __VA_ARGS__>::type
#define TICK_FUNCTION_REQUIRES(...) typename std::enable_if<(__VA_ARGS__), TICK_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE

#define TICK_CLASS_REQUIRES(...) typename std::enable_if<(__VA_ARGS__)>::type

#define TICK_REQUIRES(...)    bool TickPrivateBool ## __LINE__=true, typename std::enable_if<(TickPrivateBool##__LINE__ && __VA_ARGS__), int>::type = 0
#define TICK_REQUIRES_OC(...) bool TickPrivateBool ## __LINE__,      typename std::enable_if<(TickPrivateBool##__LINE__ && __VA_ARGS__), int>::type

#define TICK_MEMBER_REQUIRES(...)    template<TICK_REQUIRES(   __VA_ARGS__)>
#define TICK_MEMBER_REQUIRES_OC(...) template<TICK_REQUIRES_OC(__VA_ARGS__)>

#define TICK_PARAM_REQUIRES(...) \
typename std::enable_if< \
    (tick::detail::param_extract<decltype(__VA_ARGS__)>::value), \
    typename tick::detail::private_enum<__LINE__>::type \
>::type = tick::detail::private_enum<__LINE__>::type::na

#endif
