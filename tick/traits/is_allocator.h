/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_allocator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_ALLOCATOR_H
#define TICK_GUARD_IS_ALLOCATOR_H

/// is_allocator
/// ============
/// 
/// Description
/// -----------
/// 
/// An allocator encapsulates a memory allocation and deallocation strategy.
/// 
/// Requirements
/// ------------
/// 
/// Given:
/// 
/// * `T`, a cv-unqualified object type
/// * `A`, an Allocator type for type `T`
/// * `a`, an object of type `A`
/// * `B`, the corresponding allocator type for some cv-unqualified
///    object type `U` (as obtained by rebinding `A`)
/// * `ptr`, a value of type `allocator_traits<A>::pointer`, obtained
///    by calling `allocator_traits<A>::allocate()`
/// * `cptr`, a value of type `allocator_traits<A>::const_pointer`,
///    obtained by conversion from `ptr`
/// * `vptr`, a value of type `allocator_traits<A>::void_pointer`,
///    obtained by conversion from `ptr`
/// * `cvptr`, a value of type
///    `allocator_traits<A>::const_void_pointer`, obtained by conversion
///    from `cptr` or from `vptr`
/// * `xptr`, a dereferencable pointer to some cv-unqualified object type
///    `X`
/// * `n`, a value of type `allocator_traits<A>::size_type`
/// 
/// +-----------------------------+-------------------------------+--------------------------+
/// | Expression                  | Requirements                  | Return type              |
/// +=============================+===============================+==========================+
/// | `A::pointer` (optional)     | Satisfies                     |                          |
/// |                             | `is_nullable_pointer`         |                          |
/// |                             | and                           |                          |
/// |                             | `is_random_access_iterator`   |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A::const_pointer`          | `A::pointer` is               |                          |
/// | (optional)                  | convertible to                |                          |
/// |                             | `A::const_pointer`.           |                          |
/// |                             | Satisfies                     |                          |
/// |                             | `is_nullable_pointer`         |                          |
/// |                             | and                           |                          |
/// |                             | `is_random_access_iterator`   |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A::void_pointer`           | `A::pointer` is               |                          |
/// | (optional)                  | convertible to                |                          |
/// |                             | `A::void_pointer`             |                          |
/// |                             | `B::void_pointer` and         |                          |
/// |                             | `A::void_pointer` are         |                          |
/// |                             | the same type. Satisfies      |                          |
/// |                             | `is_nullable_pointer`         |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A::const_void_pointer`     | `A::pointer`,                 |                          |
/// | (optional)                  | `A::const_pointer`,           |                          |
/// |                             | and `A::void_pointer`         |                          |
/// |                             | are convertible to            |                          |
/// |                             | `A::const_void_pointer`       |                          |
/// |                             | `                             |                          |
/// |                             | `B::const_void_pointer`       |                          |
/// |                             | `                             |                          |
/// |                             | and                           |                          |
/// |                             | `A::const_void_pointer`       |                          |
/// |                             | `                             |                          |
/// |                             | are the same type.            |                          |
/// |                             | Satisfies                     |                          |
/// |                             | `is_nullable_pointer`         |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A::value_type`             |                               | the type `T`             |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `*ptr`                      |                               | `T&`                     |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `*cptr`                     | `*cptr` and `*ptr`            | `const T&`               |
/// |                             | identify the same object      |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `ptr->m`                    | same as `(*ptr).m`, if        | the type of `T::m`       |
/// |                             | `(*ptr).m` is                 |                          |
/// |                             | well-defined                  |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `cptr->m`                   | same as `(*cptr).m`, if       | the type of `T::m`       |
/// |                             | `(*cptr).m` is                |                          |
/// |                             | well-defined                  |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `static_cast <A::pointer>   | `static_cast <A::pointer>     | `A::pointer`             |
/// | (vptr)`                     | (vptr)                        |                          |
/// |                             | == ptr`                       |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `static_cast                | `static_cast                  | `A::const_pointer`       |
/// | <A::const_pointer>(cvptr)`  | <A::const_pointer>(vptr)      |                          |
/// |                             | == cptr`                      |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `a.allocate(n)`             | allocates storage             | `A::pointer`             |
/// |                             | suitable for `n`              |                          |
/// |                             | objects of type `T`,          |                          |
/// |                             | but does not construct        |                          |
/// |                             | them. May throw               |                          |
/// |                             | exceptions.                   |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `a.deallocate(ptr, n)`      | deallocates storage           | (not used)               |
/// |                             | pointed to `ptr`,             |                          |
/// |                             | which must be a value         |                          |
/// |                             | returned by a previous        |                          |
/// |                             | call to allocate that         |                          |
/// |                             | has not been invalidated      |                          |
/// |                             | by an intervening call        |                          |
/// |                             | to `deallocate`. `n`          |                          |
/// |                             | must match the value          |                          |
/// |                             | previously passed to          |                          |
/// |                             | `allocate`. Does not          |                          |
/// |                             | throw exceptions.             |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `a.max_size ()` (optional)  | the largest value that        | `A::size_type`           |
/// |                             | can be passed to              |                          |
/// |                             | `A::allocate()`               |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `a1 == a2`                  | returns true only if the      | `bool`                   |
/// |                             | storage allocated by the      |                          |
/// |                             | allocator `a1` can be         |                          |
/// |                             | deallocated through           |                          |
/// |                             | `a2`. Establishes             |                          |
/// |                             | reflexive, symmetric,         |                          |
/// |                             | and transitive                |                          |
/// |                             | relationship. Does not        |                          |
/// |                             | throw exceptions.             |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `a1 != a2`                  | same as `!(a1==a2)`           | `bool`                   |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A a1(a)`                   | Copy-constructs `a1`          |                          |
/// | `A a1 = a`                  | such that `a1 == a`. Does     |                          |
/// |                             | not throw exceptions.         |                          |
/// |                             | (Note: every                  |                          |
/// |                             | allocator also                |                          |
/// |                             | satisfies                     |                          |
/// |                             | `is_copy_constructible`       |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A a(b)`                    | Constructs `a` such           |                          |
/// |                             | that `B(a)==b` and            |                          |
/// |                             | `A(b)==a`. Does not throw     |                          |
/// |                             | exceptions. (Note: this       |                          |
/// |                             | implies that all              |                          |
/// |                             | allocators related by         |                          |
/// |                             | rebind maintain each          |                          |
/// |                             | other's resources, such       |                          |
/// |                             | as memory pools)              |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A a1(std::move(a))`        | Constructs `a1` such          |                          |
/// | `A a1 = std::move(a)`       | that it equals the prior      |                          |
/// |                             | value of `a`. Does not        |                          |
/// |                             | throw exceptions.             |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// | `A a(std::move(b))`         | Constructs `a` such           |                          |
/// |                             | that it equals the prior      |                          |
/// |                             | value of `A(b)`. Does         |                          |
/// |                             | not throw exceptions.         |                          |
/// +-----------------------------+-------------------------------+--------------------------+
/// 
/// 
/// Synopsis
/// --------
/// 
///     TICK_TRAIT(is_allocator,
///         is_copy_constructible<_>,
///         is_equality_comparable<_>
///     )
///     {
///         template<class A>
///         auto require(const A& a)-> valid<
///             decltype(returns<is_nullable_pointer<_>, is_random_access_iterator<_>>(as_mutable(a).allocate(std::declval<std::size_t>()))),
///             decltype(returns<typename A::value_type&>(*(as_mutable(a).allocate(std::declval<std::size_t>()))))
///         >;
///     };
/// 

#include <tick/builder.h>
#include <tick/traits/is_nullable_pointer.h>
#include <tick/traits/is_random_access_iterator.h>

namespace tick {

TICK_TRAIT(is_allocator,
    is_copy_constructible<_>,
    is_equality_comparable<_>
)
{
    template<class A>
    auto require(const A& a)-> valid<
        TICK_RETURNS(as_mutable(a).allocate(std::declval<std::size_t>()), is_nullable_pointer<_>, is_random_access_iterator<_>),
        TICK_RETURNS(*(as_mutable(a).allocate(std::declval<std::size_t>())), typename A::value_type&)
    >;
};

}

#endif
