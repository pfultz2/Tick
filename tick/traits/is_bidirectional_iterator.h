/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_bidirectional_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_BIDIRECTIONAL_ITERATOR_H
#define TICK_GUARD_IS_BIDIRECTIONAL_ITERATOR_H

/// is_bidirectional_iterator
/// =========================
/// 
/// Description
/// -----------
/// 
/// A bidirectional iterator is a forward iterator that can be moved in both
/// directions (i.e. incremented and decremented).
/// 
/// Requirements
/// ------------
/// 
///  The type `It` satisfies `is_bidirectional_iterator` if
///  
///  * The type It satisfies [`is_forward_iterator`](is_forward_iterator)
///  
///  And, given
///  
///  * `a` and `b`, iterators of type `It`
///  * `reference`, the type denoted by `std::iterator_traits<It>::reference`
///  
///  The following expressions must be valid and have their specified effects
///  
///  +--------------------+------------------------+------------------------------------+
///  | Expression         | Return                 | Equivalent                         |
///  |                    |                        | expression                         |
///  +====================+========================+====================================+
///  | `--a`              | `It&`                  |                                    |
///  +--------------------+------------------------+------------------------------------+
///  | `a00`              | convertible to `It&`   | `It temp = a; --a; return temp;`   |
///  +--------------------+------------------------+------------------------------------+
///  | `*a--`             | `reference`            |                                    |
///  +--------------------+------------------------+------------------------------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_bidirectional_iterator, is_forward_iterator<_>)
///     {
///         template<class I>
///         auto require(I&& i) -> valid<
///             returns<typename std::add_lvalue_reference<I>::type>(--i),
///             returns<I>(i--),
///             returns<typename iterator_traits<I>::reference>(*i--)
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/traits/is_forward_iterator.h>

namespace tick {

TICK_TRAIT(is_bidirectional_iterator, is_forward_iterator<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(--i, typename std::add_lvalue_reference<I>::type),
        TICK_RETURNS(i--, I),
        TICK_RETURNS(*i--, typename iterator_traits<I>::reference)
    >;
};

}

#endif
