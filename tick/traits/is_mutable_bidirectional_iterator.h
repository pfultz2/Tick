/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    is_mutable_bidirectional_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_IS_MUTABLE_BIDIRECTIONAL_ITERATOR_H
#define TICK_GUARD_IS_MUTABLE_BIDIRECTIONAL_ITERATOR_H

#include <tick/builder.h>
#include <tick/traits/is_bidirectional_iterator.h>
#include <tick/traits/is_output_iterator.h>

namespace tick {

TICK_TRAIT(is_mutable_bidirectional_iterator, is_bidirectional_iterator<_>, is_output_iterator<_>)
{};

}

#endif
