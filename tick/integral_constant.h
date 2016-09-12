/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    integral_constant.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_INTEGRAL_CONSTANT_H
#define TICK_GUARD_INTEGRAL_CONSTANT_H

#include <tick/detail/using.h>
#include <type_traits>
#include <iso646.h>

namespace tick {

namespace detail {
// struct integral_constant_base {};

template<class... Ts>
struct holder
{
    typedef void type;
};

template<class T, class=void>
struct is_integral_constant
: std::false_type
{};

template<class T>
struct is_integral_constant<T, typename holder<
    typename T::tick_integral_constant_tag
>::type>
: std::true_type
{};

}

template<class T, T v>
struct integral_constant
: std::integral_constant<T, v>
{
    constexpr integral_constant()
    {}

    typedef void tick_integral_constant_tag;

    template<class X, typename std::enable_if<(
        std::is_same<T, typename X::value_type>::value and X::value == v
    )>::type>
    constexpr integral_constant(X)
    {}

    constexpr operator T() const noexcept { return v; }
    constexpr T operator()() const noexcept { return v; }
};

#define TICK_INTEGRAL_CONSTANT_BINARY_OP(op) \
template<class T, T v, class U, U w> \
constexpr inline integral_constant<decltype(v op w), (v op w)> \
operator op(integral_constant<T, v>, integral_constant<U, w>) noexcept \
{ \
    return {}; \
} \
template<class T, T v, class U, class=typename std::enable_if<!detail::is_integral_constant<U>::value>::type> \
constexpr inline integral_constant<decltype(v op U::value), (v op U::value)> \
operator op(integral_constant<T, v>, U) noexcept \
{ \
    return {}; \
} \
template<class T, T v, class U, class=typename std::enable_if<!detail::is_integral_constant<U>::value>::type> \
constexpr inline integral_constant<decltype(U::value op v), (U::value op v)> \
operator op(U, integral_constant<T, v>) noexcept \
{ \
    return {}; \
}

#ifdef _MSC_VER
#define TICK_MVC_PRIMITIVE_CAT(op, x) op ## x
#define TICK_MSVC_CAT(op, x) TICK_MVC_PRIMITIVE_CAT(op, x)
#define TICK_INTEGRAL_CONSTANT_UNARY_OP_F TICK_MSVC_CAT(op_, __LINE__)
#define TICK_INTEGRAL_CONSTANT_UNARY_OP(op) \
template<class T> constexpr T TICK_INTEGRAL_CONSTANT_UNARY_OP_F (T v) { return op v; } \
template<class T, T v> \
constexpr inline integral_constant<T, TICK_INTEGRAL_CONSTANT_UNARY_OP_F(v)> \
operator op(integral_constant<T, v>) noexcept \
{ \
    return {}; \
}
#else
#define TICK_INTEGRAL_CONSTANT_UNARY_OP(op) \
template<class T, T v> \
constexpr inline integral_constant<decltype(op v), (op v)> \
operator op(integral_constant<T, v>) noexcept \
{ \
    return {}; \
}
#endif

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

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template<bool V>
TICK_USING(bool_, integral_constant<bool, V>);

template<int V>
TICK_USING(int_, integral_constant<int, V>);

template<long V>
TICK_USING(long_, integral_constant<long, V>);

template<std::size_t V>
TICK_USING(size_t, integral_constant<std::size_t, V>);

}

#endif
