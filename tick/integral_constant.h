/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    integral_constant.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_INTEGRAL_CONSTANT_H
#define TICK_GUARD_INTEGRAL_CONSTANT_H

#include <type_traits>


namespace tick {

template<class T, T v>
struct integral_constant
: std::integral_constant<T, v>
{
    constexpr integral_constant()
    {}

    template<class X, typename std::enable_if<(
        std::is_same<T, typename X::value_type>::value and X::value == v
    )>::type>
    constexpr integral_constant(X)
    {}

    constexpr T operator()() const noexcept { return v; }
};

#define TICK_INTEGRAL_CONSTANT_BINARY_OP(op) \
template<class T, T v, class U, U w> \
constexpr inline integral_constant<decltype(v op w), (v op w)> \
operator op(integral_constant<T, v>, integral_constant<U, w>) noexcept \
{ \
    return {}; \
}

#define TICK_INTEGRAL_CONSTANT_UNARY_OP(op) \
template<class T, T v> \
constexpr inline integral_constant<decltype(op v), (op v)> \
operator op(integral_constant<T, v>) noexcept \
{ \
    return {}; \
}

TICK_INTEGRAL_CONSTANT_BINARY_OP(+)
TICK_INTEGRAL_CONSTANT_BINARY_OP(-)
TICK_INTEGRAL_CONSTANT_BINARY_OP(*)
TICK_INTEGRAL_CONSTANT_BINARY_OP(/)
TICK_INTEGRAL_CONSTANT_BINARY_OP(%)
TICK_INTEGRAL_CONSTANT_BINARY_OP(>>)
TICK_INTEGRAL_CONSTANT_BINARY_OP(<<)
TICK_INTEGRAL_CONSTANT_BINARY_OP(>)
TICK_INTEGRAL_CONSTANT_BINARY_OP(<)
TICK_INTEGRAL_CONSTANT_BINARY_OP(<=)
TICK_INTEGRAL_CONSTANT_BINARY_OP(>=)
TICK_INTEGRAL_CONSTANT_BINARY_OP(==)
TICK_INTEGRAL_CONSTANT_BINARY_OP(!=)
TICK_INTEGRAL_CONSTANT_BINARY_OP(&)
TICK_INTEGRAL_CONSTANT_BINARY_OP(^)
TICK_INTEGRAL_CONSTANT_BINARY_OP(|)
TICK_INTEGRAL_CONSTANT_BINARY_OP(&&)
TICK_INTEGRAL_CONSTANT_BINARY_OP(||)

TICK_INTEGRAL_CONSTANT_UNARY_OP(!)
TICK_INTEGRAL_CONSTANT_UNARY_OP(~)
TICK_INTEGRAL_CONSTANT_UNARY_OP(+)
TICK_INTEGRAL_CONSTANT_UNARY_OP(-)

}

#endif