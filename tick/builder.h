/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    builder.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_BUILDER_H
#define TICK_GUARD_BUILDER_H

#include <tick/detail/matches.h>
#include <tick/detail/pp.h>
#include <tick/detail/using.h>
#include <tick/integral_constant.h>

namespace tick {

namespace detail {

struct no_check {};

template<class T>
struct id { typedef T type; };

template<template<class...> class... Templates>
struct template_holder
{
    typedef void type;
};

#ifdef _MSC_VER
template<class... Ts>
struct fast_and;

template<class T, class... Ts>
struct fast_and<T, Ts...>
: tick::integral_constant<bool, (T::value && fast_and<Ts...>::value)>
{};

template<>
struct fast_and<>
: tick::true_type
{};

template<class... Ts>
struct fast_or;

template<class T, class... Ts>
struct fast_or<T, Ts...>
: tick::integral_constant<bool, (T::value || fast_or<Ts...>::value)>
{};

template<>
struct fast_or<>
: tick::false_type
{};
#else
template<bool...> struct bool_seq {};
template<class... Traits>
TICK_USING(fast_and, bool_<
    std::is_same<
        detail::bool_seq<Traits::value...>, 
        detail::bool_seq<(Traits::value, true)...>
    >::type::value
>);

template<class... Traits>
TICK_USING(fast_or, bool_<
    !std::is_same<
        detail::bool_seq<Traits::value...>, 
        detail::bool_seq<(Traits::value && false)...>
    >::type::value
>);
#endif

template<class T>
TICK_USING(bare, std::remove_cv<typename std::remove_reference<T>::type>);

template<class T>
struct is_void
: std::is_void<T>
{};

template<class T>
struct is_void<std::is_same<T, void>>
: tick::true_type
{};

template<class T>
struct is_void<std::is_same<void, T>>
: tick::true_type
{};

struct base_requires
{
    template<class... Ts>
    int require(Ts&&...);
};

template<class T>
struct always_false
: tick::false_type
{};

template<class T, class... Us>
// Template alias only works on clang
#ifdef __clang__
using multi_match = fast_and<matches<T, Us>...>;
#else
struct multi_match
: fast_and<matches<T, Us>...>
{};
#endif

template<class T, class... Us>
struct return_matches
: fast_and<matches<T, Us>...>
{
    static_assert(!fast_or<detail::is_void<Us>...>::value, 
        "Void can't be used for returns. "
        "Checking for void on returns will always be false when the expression is void as well. "
        "Use TICK_RETURNS or has_type instead. "
    );
};

template<class... Traits>
struct base_traits
: fast_and<Traits...>
{
    typedef base_traits<Traits...> base_traits_type;
};

template<class T, class Enable=void>
struct refine_traits
{
    template<class...>
    TICK_USING(apply, base_traits<>);
};

template<class T>
struct refine_traits<T, typename detail::holder<typename T::tick_trait_base_apply_type>::type>
{
    template<class... Ts>
    TICK_USING_TYPENAME(apply, T::template tick_trait_base_apply<Ts...>);
};

struct any
{
    any()
    {}
    template<typename T>
    any(T &&)
    {}
};

template<class, class Trait, class... Ts>
struct apply_refinements
{
    typedef typename refine_traits<Trait>::template apply<Ts...> type;
};
struct evaluatable {};

#if TICK_HAS_TEMPLATE_ALIAS
#define TICK_LAZY_EVAL_EXPR(...) decltype(__VA_ARGS__)
#define TICK_EVAL_USING(name, ...) using name = typename __VA_ARGS__::type
#else
#define TICK_EVAL_USING(name, ...) struct name : tick::detail::lazy<__VA_ARGS__> {}

template<class T>
struct lazy
: evaluatable, T
{};

template<class T>
struct lazy_eval
: std::conditional<std::is_base_of<evaluatable, T>::value,
    T,
    id<T>
>::type
{};

#define TICK_LAZY_EVAL_EXPR(...) typename tick::detail::lazy_eval<decltype(__VA_ARGS__)>::type
#endif

template<class...Ts>
auto models_(any) -> false_type;

#if TICK_LEGACY_GCC
template<class...Ts, class Trait>
auto models_(Trait &&) -> typename apply_refinements<
    TICK_LAZY_EVAL_EXPR(std::declval<Trait>().template require<Ts...>(std::declval<Ts>()...)), 
    Trait, Ts...>::type;
#else
template<class...Ts, class Trait,
    class = TICK_LAZY_EVAL_EXPR(std::declval<Trait>().template require<Ts...>(std::declval<Ts>()...))>
auto models_(Trait &&) -> typename refine_traits<Trait>::template apply<Ts...>;
#endif
}

class ops : public tick::local_placeholders
{
    struct private_type {};
    template<bool B>
    TICK_USING(private_enable_if, std::enable_if<B, private_type>);

    template<class T, class... Us> 
    TICK_USING(has_type_, private_enable_if<detail::multi_match<T, Us...>::value>);
    template<class T> 
    TICK_USING(is_true_, private_enable_if<T::value>);
    template<class T> 
    TICK_USING(is_false_, private_enable_if<not T::value>);
    template<bool V> 
    TICK_USING(is_true_c_, private_enable_if<V>);
    template<bool V> 
    TICK_USING(is_false_c_, private_enable_if<not V>);

public:
#if TICK_HAS_TEMPLATE_ALIAS
    template<class...>
    struct valid {};
#else
    template<class... Ts>
    struct valid;
    template<class T, class=void>
    struct valid_expr 
    : detail::evaluatable    
    {};
    template<class... Ts>
    struct valid_expr<valid<Ts...>, typename detail::holder<
        typename detail::lazy_eval<Ts>::type...
    >::type>
    : detail::evaluatable
    {
        typedef valid<Ts...> type;
    };

    template<class... Ts>
    struct valid
    : valid_expr<valid<Ts...>>
    {};
#endif

    template<class... Ts, class U>
    static auto returns(U &&) ->
        typename std::enable_if<tick::detail::return_matches<U, Ts...>::value, int>::type;

#define TICK_RETURNS(expr, ...) has_type<decltype(expr), __VA_ARGS__>

template<class T, class... Ts>
TICK_EVAL_USING(has_type, has_type_<T, Ts...>);

template<class T>
TICK_EVAL_USING(is_true, is_true_<T>);

template<class T>
TICK_EVAL_USING(is_false, is_false_<T>);

template<bool V>
TICK_EVAL_USING(is_true_c, is_true_c_<V>);

template<bool V>
TICK_EVAL_USING(is_false_c, is_false_c_<V>);


// Deprecated macros
#define TICK_HAS_TYPE(...) has_type<typename __VA_ARGS__>
#define TICK_IS_TRUE(...) is_true<__VA_ARGS__>
#define TICK_IS_FALSE(...) is_false<__VA_ARGS__>
#define TICK_IS_TRUE_C(...) is_true_c<(__VA_ARGS__)>
#define TICK_IS_FALSE_C(...) is_false_c<(__VA_ARGS__)>


    template<template<class...> class Template>
    class has_template {};

    template<class T>
    static const T& as_const(const T&);

    template<class T>
    static T& as_mutable(const T&);

};

template<class... Lambdas>
struct refines
{
    typedef refines<Lambdas...> tick_trait_refinements;
    typedef void tick_trait_base_apply_type;
    template<class... Ts>
    struct tick_trait_base_apply
    : detail::base_traits<typename tick::detail::replace_args<Lambdas, Ts...>::type...>
    {
    };
};

template<class T, class Enable = void>
struct base_traits_type
{
    typedef detail::base_traits<> type;
};

template<class T>
struct base_traits_type<T, typename detail::holder<
    typename T::base_traits_type
>::type>
{
    typedef typename T::base_traits_type type;
};

template<class Trait, class... Ts>
struct models 
#if TICK_LEGACY_GCC
: detail::id<decltype(detail::models_<Ts...>(std::declval<Trait>()))>::type
#else
: decltype(detail::models_<Ts...>(std::declval<Trait>()))
#endif
{};

template<class Trait>
struct models<Trait, detail::no_check>
{
    typedef Trait type;
};

#ifdef _MSC_VER
#define TICK_MSVC_CONSTRUCT(name) constexpr name() {}
#else
#define TICK_MSVC_CONSTRUCT(name)
#endif

#define TICK_TRAIT_REFINES(name, ...) \
struct tick_private_trait_base_ ## name : tick::ops, tick::local_quote \
{ typedef tick::refines<__VA_ARGS__> type; }; \
struct tick_private_trait_ ## name; \
template<class... T> \
struct name \
: tick::models<tick_private_trait_ ## name, T...> \
{ TICK_MSVC_CONSTRUCT(name) }; \
struct tick_private_trait_ ## name \
: tick::detail::base_requires, tick::ops, tick_private_trait_base_ ## name::type

#define TICK_TRAIT(...) \
    TICK_PP_EXPAND( \
        TICK_TRAIT_REFINES TICK_IIF(TICK_PP_ARGS_IS_SINGLE(__VA_ARGS__)) \
        ( \
            (__VA_ARGS__,), \
            (__VA_ARGS__)) \
        )

// Add support for zlang
#define ZLANG_tick_trait (TICK_TRAIT)

}

#endif
