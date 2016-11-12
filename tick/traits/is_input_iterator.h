/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_input_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_INPUT_ITERATOR_H
#define TICK_GUARD_IS_INPUT_ITERATOR_H

/// is_input_iterator
/// =================
/// 
/// Description
/// -----------
/// 
/// An input iterator is an iterator that can read from the pointed-to element.
/// Input iterators only guarantee validity for single pass algorithms: once an
/// inputIterator `i` has been incremented, all copies of its previous value may
/// be invalidated.
/// 
/// Requirements
/// ------------
/// 
/// The type `It` satisfies InputIterator if
/// 
/// * The type `It` satisfies [`is_iterator`](is_iterator)
/// * The type `It` satisfies [`is_equality_comparable`](is_equality_comparable)
/// 
/// And, given
/// 
/// * `i` and `j`, values of type `It` or const It
/// * `reference`, the type denoted by `std::iterator_traits<It>::reference`
/// * `value_type`, the type denoted by `std::iterator_traits<It>::value_type`
/// 
/// The following expressions must be valid and have their specified effects
/// 
/// +--------------------+--------------------+----------------------+----------------------+
/// | Expression         | Return             | Equivalent           | Notes                |
/// |                    |                    | expression           |                      |
/// +====================+====================+======================+======================+
/// | `i != j`           | contextually       | `!(i == j)`          | **Precondition**:    |
/// |                    | convertible to     |                      | `(i, j)` is in the   |
/// |                    | `bool`             |                      | domain of `==`.      |
/// +--------------------+--------------------+----------------------+----------------------+
/// | `*i`               | reference,         | If `i == j` and      | **Precondition**:    |
/// |                    | convertible to     | `(i, j)` is in the   | `i` is               |
/// |                    | `value_type`       | domain of `==` then  | dereferenceable.     |
/// |                    |                    | this is equivalent   |                      |
/// |                    |                    | to `*j`.             | The expression       |
/// |                    |                    |                      | `(void)*i`, `*i` is  |
/// |                    |                    |                      | equivalent to `*i`.  |
/// +--------------------+--------------------+----------------------+----------------------+
/// | `i->m`             |                    | `(*i).m`             | **Precondition**:    |
/// |                    |                    |                      | `i` is               |
/// |                    |                    |                      | dereferenceable.     |
/// +--------------------+--------------------+----------------------+----------------------+
/// | `++i`              | `It&`              |                      | **Precondition**:    |
/// |                    |                    |                      | `i` is               |
/// |                    |                    |                      | dereferenceable.     |
/// |                    |                    |                      |                      |
/// |                    |                    |                      | **Postcondition**:   |
/// |                    |                    |                      | `i` is               |
/// |                    |                    |                      | dereferenceable or   |
/// |                    |                    |                      | `i` is past-the-end. |
/// |                    |                    |                      |                      |
/// |                    |                    |                      | **Postcondition**:   |
/// |                    |                    |                      | Any copies of the    |
/// |                    |                    |                      | previous value of    |
/// |                    |                    |                      | `i` are no longer    |
/// |                    |                    |                      | required to be       |
/// |                    |                    |                      | either               |
/// |                    |                    |                      | dereferenceable or   |
/// |                    |                    |                      | to be in the         |
/// |                    |                    |                      | domain of `==`.      |
/// +--------------------+--------------------+----------------------+----------------------+
/// | `(void)i++`        |                    | `(void)++i`          |                      |
/// +--------------------+--------------------+----------------------+----------------------+
/// | `*i++`             | convertible to     | `value_type x = *i;` |                      |
/// |                    | `value_type`       | `++i;`               |                      |
/// |                    |                    | `return x;`          |                      |
/// |                    |                    |                      |                      |
/// +--------------------+--------------------+----------------------+----------------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_input_iterator, 
///         is_iterator<_>, 
///         is_equality_comparable<_>
///     )
///     {
///         template<class I>
///         auto require(I&& i) -> valid<
///             returns<typename iterator_traits<I>::value_type>(*i),
///             returns<typename iterator_traits<I>::value_type>(*i++)
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/traits/is_iterator.h>
#include <tick/traits/is_equality_comparable.h>

namespace tick {

TICK_TRAIT(is_input_iterator, is_iterator<_>, is_equality_comparable<_>)
{
    template<class I>
    auto require(I&& i) -> valid<
        TICK_RETURNS(*i, typename iterator_traits<I>::value_type),
        TICK_RETURNS(*i++, typename iterator_traits<I>::value_type)
    >;
};

}

#endif
