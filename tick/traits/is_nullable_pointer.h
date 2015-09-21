/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_nullable_pointer.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_NULLABLE_POINTER_H
#define TICK_GUARD_IS_NULLABLE_POINTER_H

#include <tick/builder.h>
#include <tick/traits/is_equality_comparable.h>
#include <tick/traits/is_default_constructible.h>
#include <tick/traits/is_copy_constructible.h>
#include <tick/traits/is_copy_assignable.h>
#include <tick/traits/is_destructible.h>
#include <tick/traits/is_equality_comparable.h>

namespace tick {

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

}

#endif
