/*=============================================================================
    Copyright (c) 2016 Paul Fultz II
    using.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_USING_H
#define TICK_GUARD_USING_H

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

#if TICK_HAS_TEMPLATE_ALIAS
#define TICK_USING(name, ...) using name = __VA_ARGS__
#define TICK_USING_TYPENAME(name, ...) using name = typename __VA_ARGS__
#else
#define TICK_USING_TYPENAME(name, ...) struct name : __VA_ARGS__ {}
#define TICK_USING(name, ...) struct name : __VA_ARGS__ {}
#endif

#endif
