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
#include <tick/integral_constant.h>


#if defined (__GNUC__) && !defined (__clang__)
#    if __GNUC__ == 4 && __GNUC_MINOR__ < 7
#        define TICK_LEGACY_GCC 1
#    else
#        define TICK_LEGACY_GCC 0
#    endif
#else
#define TICK_LEGACY_GCC 0
#endif


#ifndef TICK_HAS_TEMPLATE_ALIAS
#   if TICK_LEGACY_GCC
#       define TICK_HAS_TEMPLATE_ALIAS 0
#   else
#       define TICK_HAS_TEMPLATE_ALIAS 1
#   endif
#endif

namespace tick {

namespace detail {

struct no_check {};

template<class T>
struct id { typedef T type; };

template<class T1 = void, class T2 = void, class T3 = void, class T4 = void, class T5 = void>
struct holder
{
    typedef void type;
};

template<template<class...> class... Templates>
struct template_holder
{
    typedef void type;
};

template<class T>
struct bare
: std::remove_cv<typename std::remove_reference<T>::type>
{};

template<class T>
struct is_void
: std::is_same<T, void>
{};

template<class T>
struct is_void<std::is_same<T, void>>
: std::true_type
{};

template<class T>
struct is_void<std::is_same<void, T>>
: std::true_type
{};

struct base_requires
{
    template<class... Ts>
    int require(Ts&&...);
};

template<class T>
struct always_false
: tick::integral_constant<bool, false>
{};

template<class T, class U>
struct return_matches
: detail::matches<U,T>
{
    static_assert(!detail::is_void<T>::value, 
        "Void can't be used for returns. "
        "Checking for void on returns will always be false when the expression is void as well. "
        "Use TICK_RETURNS or has_type instead. "
    );
};

template<bool...> struct bool_seq {};

template<class... Traits>
struct base_traits
: tick::integral_constant<bool, 
    std::is_same<
        detail::bool_seq<Traits::value...>, 
        detail::bool_seq<(Traits::value, true)...>
    >::type::value
>
{
    typedef base_traits<Traits...> base_traits_type;
};

template<class T, class Enable=void>
struct refine_traits
{
    template<class... Ts>
    struct apply
    : base_traits<>
    {
    };
};

template<class T>
struct refine_traits<T, typename detail::template_holder<T::template tick_trait_base_apply>::type>
{
    template<class... Ts>
    struct apply
    : T::template tick_trait_base_apply<Ts...>
    {

    };
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

template<class...Ts>
auto models_(any) -> false_type;

#if TICK_LEGACY_GCC
template<class...Ts, class Trait>
auto models_(Trait &&) -> typename apply_refinements<
    decltype(std::declval<Trait>().template require<Ts...>(std::declval<Ts>()...)), 
    Trait, Ts...>::type;
#else
template<class...Ts, class Trait,
    class = decltype(std::declval<Trait>().template require<Ts...>(std::declval<Ts>()...))>
auto models_(Trait &&) -> typename refine_traits<Trait>::template apply<Ts...>;
#endif
}

class ops : public tick::local_placeholders
{
    struct private_type {};
    template<bool B>
    struct private_enable_if
    : std::enable_if<B, private_type>
    {};
    template<class T, class U> 
    struct has_type_ : private_enable_if<detail::matches<T,U>::value> {};
    template<class T> 
    struct is_true_ : private_enable_if<T::value> {};
    template<class T> 
    struct is_false_ : private_enable_if<not T::value> {};
    template<bool V> 
    struct is_true_c_ : private_enable_if<V> {};
    template<bool V> 
    struct is_false_c_ : private_enable_if<not V> {};

public:

    template<class...>
    struct valid {};

    template<typename T, typename U>
    static auto returns(U &&) ->
        typename std::enable_if<detail::return_matches<T,U>::value, int>::type;

// A macro to provide better compatibility for gcc 4.6
#define TICK_RETURNS(expr, ...) typename std::enable_if<tick::detail::matches<decltype(expr),__VA_ARGS__>::value, int>::type


#if TICK_HAS_TEMPLATE_ALIAS
template<class T, class U=void>
using has_type = typename has_type_<T, U>::type; 

template<class T>
using is_true = typename is_true_<T>::type; 

template<class T>
using is_false = typename is_false_<T>::type; 

template<bool V>
using is_true_c = typename is_true_c_<V>::type; 

template<bool V>
using is_false_c = typename is_false_c_<V>::type; 

#define TICK_HAS_TYPE(...) has_type<typename __VA_ARGS__>
#define TICK_IS_TRUE(...) is_true<__VA_ARGS__>
#define TICK_IS_FALSE(...) is_false<__VA_ARGS__>
#define TICK_IS_TRUE_C(...) is_true_c<__VA_ARGS__>
#define TICK_IS_FALSE_C(...) is_false_c<__VA_ARGS__>
#else
template<class T, class U=void, class Enable=typename has_type_<T, U>::type>
struct has_type {}; 

template<class T, class Enable=typename is_true_<T>::type>
struct is_true {}; 

template<class T, class Enable=typename is_false_<T>::type>
struct is_false {}; 

template<bool V, class Enable=typename is_true_c_<V>::type>
struct is_true_c {}; 

template<bool V, class Enable=typename is_false_c_<V>::type>
struct is_false_c {}; 

#define TICK_HAS_TYPE(...) decltype(has_type<typename __VA_ARGS__>())
#define TICK_IS_TRUE(...) decltype(is_true<__VA_ARGS__>())
#define TICK_IS_FALSE(...) decltype(is_false<__VA_ARGS__>())
#define TICK_IS_TRUE_C(...) decltype(is_true_c<__VA_ARGS__>())
#define TICK_IS_FALSE_C(...) decltype(is_false_c<__VA_ARGS__>())
#endif


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

#define TICK_TRAIT_REFINES(name, ...) \
struct tick_private_trait_base_ ## name : tick::ops, tick::local_quote \
{ typedef tick::refines<__VA_ARGS__> type; }; \
struct tick_private_trait_ ## name; \
template<class... T> \
struct name \
: tick::models<tick_private_trait_ ## name, T...> \
{}; \
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
