/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    traits.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_TRAITS_H
#define TICK_GUARD_TRAITS_H

#include <tick/builder.h>
#include <tick/requires.h>
#include <iterator>

namespace tick_adl {

using std::swap;
using std::begin;
using std::end;

template<class T, class U>
auto swap(T&& x, U&& y) -> decltype(swap(std::forward<T>(x), std::forward<U>(y)));

template<class T>
auto begin(T&& x) -> decltype(begin(std::forward<T>(x)));

template<class T>
auto end(T&& x) -> decltype(end(std::forward<T>(x)));

}

namespace tick {

template<class T>
struct bare
: std::remove_cv<typename std::remove_reference<T>::type>
{};

#if TICK_HAS_TEMPLATE_ALIAS

template<class T>
using bare_t = typename bare<T>::type;

#endif

template<class T>
struct is_default_constructible
: integral_constant<bool, std::is_default_constructible<T>::value>
{};

template<class T>
struct is_copy_constructible
: integral_constant<bool, std::is_copy_constructible<T>::value>
{};

template<class T>
struct is_copy_assignable
: integral_constant<bool, std::is_copy_assignable<T>::value>
{};

template<class T>
struct is_move_constructible
: integral_constant<bool, std::is_move_constructible<T>::value>
{};

template<class T>
struct is_move_assignable
: integral_constant<bool, std::is_move_assignable<T>::value>
{};

template<class T>
struct is_destructible
: integral_constant<bool, std::is_destructible<T>::value>
{};

// We use trival for trivally copyable since isn't implemented yet in gcc
template<class T>
struct is_trivially_copyable
: integral_constant<bool, std::is_trivial<T>::value>
{};

template<class T>
struct is_trivial
: integral_constant<bool, std::is_trivial<T>::value>
{};

template<class T>
struct is_standard_layout
: integral_constant<bool, std::is_standard_layout<T>::value>
{};

template<class T>
struct is_pod
: integral_constant<bool, std::is_pod<T>::value>
{};

TICK_TRAIT(is_equality_comparable)
{
    template<class T>
    auto require(T&& x) -> valid<
        TICK_RETURNS(x == x, bool),
        TICK_RETURNS(x != x, bool)
    >;

    template<class T, class U>
    auto require(T&& x, U&& y) -> valid<
        decltype(require(std::forward<T>(x))),
        decltype(require(std::forward<U>(y))),
        TICK_RETURNS(x == y, bool),
        TICK_RETURNS(x != y, bool),
        TICK_RETURNS(y == x, bool),
        TICK_RETURNS(y != x, bool)
    >;
};

TICK_TRAIT(is_less_than_comparable)
{
    template<class T>
    auto require(T&& x) -> valid<
        TICK_RETURNS(x < x, bool)
    >;

    template<class T, class U>
    auto require(T&& x, U&& y) -> valid<
        TICK_RETURNS(x < x, bool),
        TICK_RETURNS(y < y, bool),
        TICK_RETURNS(x < y, bool),
        TICK_RETURNS(y < x, bool)
    >;
};

TICK_TRAIT(is_weakly_ordered)
{
    template<class T>
    auto require(T&& x) -> valid<
        TICK_RETURNS(x < x, bool),
        TICK_RETURNS(x > x, bool),
        TICK_RETURNS(x <= x, bool),
        TICK_RETURNS(x >= x, bool)
    >;

    template<class T, class U>
    auto require(T&& x, U&& y) -> valid<
        decltype(require(std::forward<T>(x))),
        decltype(require(std::forward<U>(y))),
        TICK_RETURNS(x < y, bool),
        TICK_RETURNS(y < x, bool),

        TICK_RETURNS(x > y, bool),
        TICK_RETURNS(y > x, bool),

        TICK_RETURNS(x <= y, bool),
        TICK_RETURNS(y <= x, bool),

        TICK_RETURNS(x >= y, bool),
        TICK_RETURNS(y >= x, bool)
    >;
};

TICK_TRAIT(is_totally_ordered,
    quote<is_weakly_ordered>,
    quote<is_equality_comparable>
)
{};

TICK_TRAIT(is_swappable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(tick_adl::swap(x, x))
    >;
};

TICK_TRAIT(is_nullable_pointer,
    is_equality_comparable<_>,
    is_default_constructible<_>,
    is_copy_constructible<_>,
    is_copy_assignable<_>,
    is_destructible<_>,
    std::is_constructible<_, std::nullptr_t>,
    is_equality_comparable<_, std::nullptr_t>
)
{};

// TODO: Function

TICK_TRAIT(is_predicate)
{
    template<class F, class... Ts>
    auto require(F&& f, Ts&&... xs) -> valid<
        TICK_RETURNS(f(std::forward<Ts>(xs)...), bool)
    >;
};

TICK_TRAIT(is_compare)
{
    template<class F, class T>
    auto require(F&& f, T&& x) -> valid<
        TICK_RETURNS(f(std::forward<T>(x), std::forward<T>(x)), bool)
    >;

    template<class F, class T, class U>
    auto require(F&& f, T&& x, U&& y) -> valid<
        decltype(require(std::forward<T>(x))),
        decltype(require(std::forward<U>(y))),
        TICK_RETURNS(f(std::forward<T>(x), std::forward<U>(y)), bool),
        TICK_RETURNS(f(std::forward<U>(y), std::forward<T>(x)), bool)
    >;
};

namespace detail {

// A trait to detect iterator traits, since it is not SFINAE friendly
TICK_TRAIT(has_iterator_traits)
{
    template<class T,class Iterator=typename bare<T>::type>
    auto require(T&&) -> valid<
        typename Iterator::difference_type,
        typename Iterator::value_type,
        typename Iterator::pointer,
        typename Iterator::reference,
        typename Iterator::iterator_category
    >;
};

template<class T>
struct has_iterator_traits<T*>
: true_type
{};

}

template<class T, class=void>
struct iterator_traits;

template<class T>
struct iterator_traits<T, TICK_CLASS_REQUIRES(detail::has_iterator_traits<T>())>
: std::iterator_traits<typename bare<T>::type>
{};

TICK_TRAIT(is_iterator,
    is_copy_constructible<_>,
    is_copy_assignable<_>,
    is_destructible<_>,
    is_swappable<_>
)
{
    template<class I>
    auto require(I&& i) -> valid<
        // TICK_RETURNS(*i, std::is_same<_, typename iterator_traits<I>::reference>),
        TICK_RETURNS(*i, typename iterator_traits<I>::reference),
        TICK_RETURNS(++i, typename std::add_lvalue_reference<I>::type)
    >;
};

TICK_TRAIT(is_input_iterator, is_iterator<_>, is_equality_comparable<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(*i, typename iterator_traits<I>::value_type),
        TICK_RETURNS(*i++, typename iterator_traits<I>::value_type)
    >;
};

TICK_TRAIT(is_output_iterator, is_iterator<_>)
{
    template<class I, class T>
    auto require(I&& i, T&& x) -> valid<
        decltype(*i = x),
        decltype(*i++ = x),
        TICK_RETURNS(++i, typename std::add_const<typename std::add_lvalue_reference<I>::type>::type)
    >;

    template<class I>
    auto require(I&& i) -> valid<
        decltype(require(std::forward<I>(i), std::declval<typename iterator_traits<I>::value_type>()))
    >;
};

TICK_TRAIT(is_forward_iterator, is_input_iterator<_>, is_default_constructible<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(++i, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i++, I),
        TICK_IS_TRUE(std::is_reference<typename iterator_traits<I>::reference>),
        // TICK_RETURNS(*i, typename iterator_traits<I>::value_type&),
        TICK_RETURNS(*i++, typename iterator_traits<I>::reference)
    >;
};

TICK_TRAIT(is_mutable_forward_iterator, is_forward_iterator<_>, is_output_iterator<_>)
{};

TICK_TRAIT(is_bidirectional_iterator, is_forward_iterator<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(--i, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i--, I),
        TICK_RETURNS(*i--, typename iterator_traits<I>::reference)
    >;
};

TICK_TRAIT(is_mutable_bidirectional_iterator, is_bidirectional_iterator<_>, is_output_iterator<_>)
{};

TICK_TRAIT(is_random_access_iterator, is_bidirectional_iterator<_>, is_totally_ordered<_>)
{
    template<class I, class Number>
    auto require(I&& i, Number n) -> valid<
        TICK_RETURNS(i += n, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i -= n, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i + n, I),
        TICK_RETURNS(i - n, I),
        TICK_RETURNS(n + i, I),
        TICK_RETURNS(i - i, typename iterator_traits<I>::difference_type),
        TICK_RETURNS(i[n], typename iterator_traits<I>::reference),
        TICK_RETURNS(*(i + n), typename iterator_traits<I>::reference)
    >;

    template<class I>
    auto require(I&& i) -> valid<
        decltype(require(std::forward<I>(i), 0))
    >;
};

TICK_TRAIT(is_mutable_random_access_iterator, quote<is_random_access_iterator>, is_output_iterator<_>)
{};

TICK_TRAIT(is_value_swappable, is_iterator<_>)
{
    template<class T>
    auto require(T&&) -> valid<
        TICK_IS_TRUE(is_swappable<typename iterator_traits<T>::value_type>)
    >;
};

TICK_TRAIT(is_range)
{
    template<class T>
    auto require(T&& x) -> valid<
        TICK_RETURNS(tick_adl::begin(std::forward<T>(x)), is_iterator<_>),
        TICK_RETURNS(tick_adl::end(std::forward<T>(x)), is_iterator<_>)
    >;
};

// TODO: Add CopyInsertable
TICK_TRAIT(is_container,
    is_equality_comparable<_>,
    is_default_constructible<_>,
    is_copy_constructible<_>,
    is_copy_assignable<_>,
    is_destructible<_>,
    is_swappable<_>,
    is_range<_>
)
{
    template<class T>
    auto require(const T& x) -> valid<
        TICK_HAS_TYPE(T::iterator, is_iterator<_>),
        TICK_HAS_TYPE(T::const_iterator, is_iterator<_>),
        TICK_HAS_TYPE(T::value_type),
        TICK_HAS_TYPE(T::reference, typename T::value_type&),
        TICK_HAS_TYPE(T::const_reference, const typename T::value_type&),
        TICK_HAS_TYPE(T::difference_type, int),
        TICK_HAS_TYPE(T::size_type, unsigned),
        TICK_RETURNS(x.size(), typename T::size_type),
        TICK_RETURNS(x.max_size(), typename T::size_type),
        TICK_RETURNS(x.empty(), bool)
    >;
};

TICK_TRAIT(is_reversible_container, is_container<_>)
{
    template<class T>
    auto require(const T& x) -> valid<
        TICK_HAS_TYPE(T::reverse_iterator, is_iterator<_>),
        TICK_HAS_TYPE(T::const_reverse_iterator, is_iterator<_>),
        TICK_RETURNS(x.crbegin(), typename T::const_reverse_iterator),
        TICK_RETURNS(x.crend(), typename T::const_reverse_iterator),
        TICK_RETURNS(x.rbegin(), typename T::const_reverse_iterator),
        TICK_RETURNS(x.rend(), typename T::const_reverse_iterator),
        TICK_RETURNS(as_mutable(x).rbegin(), typename T::reverse_iterator),
        TICK_RETURNS(as_mutable(x).rend(), typename T::reverse_iterator)
    >;
};

TICK_TRAIT(is_sequence_container, is_container<_>)
{
    template<class X, class T, class N, class Iterator>
    auto require(X& a, const T& t, N n, const Iterator& p) -> valid<
        TICK_RETURNS(a.insert(p, t), Iterator),
        // This should return an iterator too in C++11 but currently gcc does not
        // implement it
        TICK_RETURNS(a.insert(p, n, t), void),
        TICK_RETURNS(a.erase(p), Iterator),
        TICK_RETURNS(a.erase(p, p), Iterator),
        decltype(a.clear()),
        decltype(a.assign(n, t))
    >;

    template<class T>
    auto require(const T& x) -> valid<
        decltype(require(
            as_mutable(x), 
            std::declval<typename T::value_type>(), 
            std::declval<typename T::size_type>(),
            // Should be a const_iterator, but many containers don't implement
            // this correctly
            std::declval<typename T::iterator>()
        ))
    >;
};

TICK_TRAIT(is_associative_container, is_container<_>)
{
    template<class T>
    auto require(const T& x) -> valid<
        TICK_HAS_TYPE(T::key_type, is_destructible<_>),
        TICK_HAS_TYPE(T::key_compare, is_compare<_, typename T::key_type>),
        TICK_HAS_TYPE(T::value_compare, is_compare<_, typename T::value_type>),
        TICK_RETURNS(x.key_comp(), typename T::key_compare),
        TICK_RETURNS(x.value_comp(), typename T::value_compare)
    >;
};


}


#endif