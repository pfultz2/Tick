/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_output_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_OUTPUT_ITERATOR_H
#define TICK_GUARD_IS_OUTPUT_ITERATOR_H

/// is_output_iterator
/// ==================
/// 
/// Description
/// -----------
/// 
/// An output iterator is an iterator that can write to the pointed-to element.
/// 
/// Requirements
/// ------------
/// 
/// The type `X` satisfies `is_output_itearator` if
/// 
/// * The type `X` satisfies [`is_iterator`](is_iterator)
/// * `X` is a class type or a pointer type
/// 
/// And, given
/// 
/// * `o`, a value of some type that is writable to the output iterator (there may
///   be multiple types that are writable, e.g. if `operator=` may be a template.
///   There is no notion of `value_type` as for the input iterators)
/// * r, an lvalue of type `X`,
/// 
/// The following expressions must be valid and have their specified effects:
/// 
/// +--------------+--------------+--------------+----------------+----------------+----------------+
/// | Expression   | Return       | Equivalent   | Pre-condition  | Post-conditions| Notes          |
/// |              |              | expression   |                |                |                |
/// +==============+==============+==============+================+================+================+
/// | `*r = o`     | (not used)   |              | `r` is         | `r` is         | After this     |
/// |              |              |              | dereferencable | incrementable  | operation      |
/// |              |              |              |                |                | `r` is not     |
/// |              |              |              |                |                | required to    |
/// |              |              |              |                |                | be             |
/// |              |              |              |                |                | dereferenceable|
/// |              |              |              |                |                | and any        |
/// |              |              |              |                |                | copies of      |
/// |              |              |              |                |                | the previous   |
/// |              |              |              |                |                | value of       |
/// |              |              |              |                |                | `r` are no     |
/// |              |              |              |                |                | longer         |
/// |              |              |              |                |                | required to    |
/// |              |              |              |                |                | be             |
/// |              |              |              |                |                | dereferenceable|
/// |              |              |              |                |                | or             |
/// |              |              |              |                |                | incrementable. |
/// +--------------+--------------+--------------+----------------+----------------+----------------+
/// | `++r`        | `X&`         |              | `r` is         | `&r == &++r`,  | After this     |
/// |              |              |              | incrementable  | `r` is         | operation      |
/// |              |              |              |                | dereferencable | `r` is not     |
/// |              |              |              |                |                | required to    |
/// |              |              |              |                | or             | be             |
/// |              |              |              |                | past-the-end   | incrementable  |
/// |              |              |              |                |                | and any        |
/// |              |              |              |                |                | copies of      |
/// |              |              |              |                |                | the previous   |
/// |              |              |              |                |                | value of       |
/// |              |              |              |                |                | `r` are no     |
/// |              |              |              |                |                | longer         |
/// |              |              |              |                |                | required to    |
/// |              |              |              |                |                | be             |
/// |              |              |              |                |                | dereferenceable|
/// |              |              |              |                |                | ble            |
/// |              |              |              |                |                | or             |
/// |              |              |              |                |                | incrementable. |
/// +--------------+--------------+--------------+----------------+----------------+----------------+
/// | `r++`        | convertible  | `X temp = r;`|                |                |                |
/// |              | to `const X&`| `++r;`       |                |                |                |
/// |              |              | `return      |                |                |                |
/// |              |              |   temp;`     |                |                |                |
/// +--------------+--------------+--------------+----------------+----------------+----------------+
/// | `*r++ = o`   | (not used)   | `*r = o;`    |                |                |                |
/// |              |              | `++r;`       |                |                |                |
/// +--------------+--------------+--------------+----------------+----------------+----------------+
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_output_iterator, is_iterator<_>)
///     {
///         template<class I, class T>
///         static auto require(I&& i, T&& x) -> valid<
///             decltype(*i = x),
///             decltype(*i++ = x),
///             decltype(returns<typename std::add_const<typename std::add_lvalue_reference<I>::type>::type>(++i))
///         >;
///     
///         template<class I>
///         auto require(I&& i) -> valid<
///             decltype(require(std::forward<I>(i), std::declval<typename iterator_traits<I>::value_type>()))
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/traits/is_iterator.h>

namespace tick {

TICK_TRAIT(is_output_iterator, is_iterator<_>)
{
    template<class I, class T>
    static auto req_impl(I&& i, T&& x) -> valid<
        decltype(*i = x),
        decltype(*i++ = x),
        TICK_RETURNS(++i, typename std::add_const<typename std::add_lvalue_reference<I>::type>::type)
    >;

    template<class I, class T>
    auto require(I&& i, T&& x) -> valid<
        decltype(req_impl(std::forward<I>(i), std::forward<T>(x)))
    >;

    template<class I>
    auto require(I&& i) -> valid<
        decltype(req_impl(std::forward<I>(i), std::declval<typename iterator_traits<I>::value_type>()))
    >;
};

}

#endif
