/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_random_access_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_RANDOM_ACCESS_ITERATOR_H
#define TICK_GUARD_IS_RANDOM_ACCESS_ITERATOR_H

/// is_random_access_iterator
/// =========================
/// 
/// Description
/// -----------
/// 
/// A random access iterator is a bidirectional iterator that can be moved to
/// point to any element in constant time.
/// 
/// Requirements
/// ------------
/// 
/// The type `It` satisfies `is_random_access_iterator` if
/// 
/// * The type `It` satisfies [`is_bidirectional_iterator`](is_bidirectional_iterator)
/// * The type `It` satisfies [`is_totally_ordered`](is_totally_ordered)
/// 
/// And, given
/// 
/// * `value_type`, the type denoted by `std::iterator_traits<It>::value_type`
/// * `difference_type`, the type denoted by `std::iterator_traits<It>::difference_type`
/// * `reference`, the type denoted by `std::iterator_traits<It>::reference`
/// * `i`, `a`, `b`, objects of type `It` or `const It`
/// * `r`, a value of type `It&`
/// * `n`, an integer of type `difference_type`
/// 
/// The following expressions must be valid and have their specified effects
/// 
/// +--------------------+--------------------+
/// | Expression         | Return type        |
/// |                    |                    |
/// +====================+====================+
/// | `r += n`           | `It&`              |
/// +--------------------+--------------------+
/// | `a + n, n + a`     | `It`               |
/// +--------------------+--------------------+
/// | `r -= n`           | `It&`              |
/// +--------------------+--------------------+
/// | `i - n`            | `It`               |
/// +--------------------+--------------------+
/// | `b - a`            | `difference_type`  |
/// +--------------------+--------------------+
/// | `i[n]`             | convertible to     |
/// |                    | `reference`        |
/// +--------------------+--------------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_random_access_iterator, 
///         is_bidirectional_iterator<_>, 
///         is_totally_ordered<_>
///     )
///     {
///         template<class I, class Number>
///         auto require(I&& i, Number n) -> valid<
///             returns<typename std::add_lvalue_reference<I>::type>(i += n),
///             returns<typename std::add_lvalue_reference<I>::type>(i -= n),
///             returns<I>(i + n),
///             returns<I>(i - n),
///             returns<I>(n + i),
///             returns<typename iterator_traits<I>::difference_type>(i - i),
///             returns<typename iterator_traits<I>::reference>(i[n]),
///             returns<typename iterator_traits<I>::reference>(*(i + n))
///         >;
///     
///         template<class I>
///         auto require(I&& i) -> valid<
///             decltype(require(std::forward<I>(i), 0))
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/traits/is_bidirectional_iterator.h>
#include <tick/traits/is_totally_ordered.h>

namespace tick {

TICK_TRAIT(is_random_access_iterator, is_bidirectional_iterator<_>, is_totally_ordered<_>)
{
    template<class I, class Number>
    static auto req_impl(I&& i, Number n) -> valid<
        TICK_RETURNS(i += n, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i -= n, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i + n, I),
        TICK_RETURNS(i - n, I),
        TICK_RETURNS(n + i, I),
        TICK_RETURNS(i - i, typename iterator_traits<I>::difference_type),
        TICK_RETURNS(i[n], typename iterator_traits<I>::reference),
        TICK_RETURNS(*(i + n), typename iterator_traits<I>::reference)
    >;

    template<class I, class Number>
    auto require(I&& i, Number n) -> valid<
        decltype(req_impl(std::forward<I>(i), n))
    >;

    template<class I>
    auto require(I&& i) -> valid<
        decltype(req_impl(std::forward<I>(i), 0))
    >;
};

}

#endif
