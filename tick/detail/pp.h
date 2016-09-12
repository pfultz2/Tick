/*=============================================================================
    Copyright (c) 2014 Paul Fultz II
    pp.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef TICK_GUARD_PP_H
#define TICK_GUARD_PP_H

#define TICK_PP_CAT(x, y) TICK_PP_PRIMITIVE_CAT(x, y)
#define TICK_PP_PRIMITIVE_CAT(x, y) x ## y

#define TICK_PP_SEQ_ITERATE(...) TICK_PP_PRIMITIVE_SEQ_ITERATE(__VA_ARGS__)
#define TICK_PP_PRIMITIVE_SEQ_ITERATE(...) __VA_ARGS__ ## _END

// 
// TICK_PP_NARGS returns the number of args in __VA_ARGS__
// 
#define TICK_PP_NARGS(...) \
         TICK_PP_DETAIL_NARG((__VA_ARGS__,TICK_PP_DETAIL_RSEQ_N()))

#define TICK_PP_DETAIL_NARG(args) \
        TICK_PP_DETAIL_ARG_N args

#define TICK_PP_DETAIL_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define TICK_PP_DETAIL_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 


//
// TICK_PP_IS_PAREN is used to detect if the first token is a parenthesis.
// It expands to 1 if it is, otherwise it expands to 0.
//
#define TICK_PP_IS_PAREN(x) TICK_PP_IS_PAREN_CHECK(TICK_PP_IS_PAREN_PROBE x)
#define TICK_PP_IS_PAREN_CHECK(...) TICK_PP_IS_PAREN_CHECK_N(__VA_ARGS__,0)
#define TICK_PP_IS_PAREN_PROBE(...) ~, 1,
#define TICK_PP_IS_PAREN_CHECK_N(x, n, ...) n

//
// TICK_PP_IS_1 is used to detect if the first token is a 1.
// It expands to 1 if it is, otherwise it expands to 0.
//
#define TICK_PP_IS_1(x) TICK_PP_IS_PAREN(TICK_PP_CAT(TICK_PP_IS_1_PROBE_, x))
#define TICK_PP_IS_1_PROBE_1 ()

#define TICK_PP_ARGS_IS_SINGLE(...) TICK_PP_IS_1(TICK_PP_NARGS(__VA_ARGS__))

#define TICK_PP_EXPAND(...) __VA_ARGS__

#define TICK_IIF(c) TICK_PP_PRIMITIVE_CAT(TICK_IIF_, c)
#define TICK_IIF_0(t, ...) __VA_ARGS__
#define TICK_IIF_1(t, ...) t

#endif
