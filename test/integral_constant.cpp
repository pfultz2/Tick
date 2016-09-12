

#include "test.h"
#include <tick/integral_constant.h>

TICK_STATIC_TEST_CASE()
{
    struct true_type
    : tick::integral_constant<bool, true>
    {};
    static_assert(true_type(), "Failed");
    struct false_type
    : tick::integral_constant<bool, false>
    {};
    static_assert(!false_type(), "Failed");

    typedef tick::integral_constant<int, 1> one_type;
    typedef tick::integral_constant<int, 2> two_type;

    typedef decltype(!true_type()) not_true_type;
    static_assert(not_true_type::value == false, "Failed");

    typedef decltype(true_type() && false_type()) true_type_and_false_type;
    static_assert(true_type_and_false_type::value == false, "Failed");
    typedef decltype(true_type() || false_type()) true_type_or_false_type;
    static_assert(true_type_or_false_type::value == true, "Failed");

    typedef decltype(!two_type()) not_two_type;
    static_assert(not_two_type::value == (!2), "Failed");

// GCC confuses the complement operator with the destructor
#ifdef __clang__
    typedef decltype(~two_type()) compl_two_type;
    static_assert(compl_two_type::value == (~2), "Failed");
#endif

    typedef decltype(+two_type()) unary_plus_two_type;
    static_assert(unary_plus_two_type::value == (+2), "Failed");
    typedef decltype(-two_type()) unary_subtract_two_type;
    static_assert(unary_subtract_two_type::value == (-2), "Failed");

    typedef decltype(one_type() + two_type()) one_type_add_two_type;
    static_assert(one_type_add_two_type::value == (1 + 2), "Failed");
    typedef decltype(one_type() - two_type()) one_type_subtract_two_type;
    static_assert(one_type_subtract_two_type::value == (1 - 2), "Failed");
    typedef decltype(one_type() * two_type()) one_type_multiply_two_type;
    static_assert(one_type_multiply_two_type::value == (1 * 2), "Failed");
    typedef decltype(one_type() / two_type()) one_type_divide_two_type;
    static_assert(one_type_divide_two_type::value == (1 / 2), "Failed");
    typedef decltype(one_type() % two_type()) one_type_remainder_two_type;
    static_assert(one_type_remainder_two_type::value == (1 % 2), "Failed");
    typedef decltype(one_type() >> two_type()) one_type_shift_right_two_type;
    static_assert(one_type_shift_right_two_type::value == (1 >> 2), "Failed");
    typedef decltype(one_type() << two_type()) one_type_shift_left_two_type;
    static_assert(one_type_shift_left_two_type::value == (1 << 2), "Failed");
    typedef decltype(one_type() > two_type()) one_type_greater_than_two_type;
    static_assert(one_type_greater_than_two_type::value == (1 > 2), "Failed");
    typedef decltype(one_type() < two_type()) one_type_less_than_two_type;
    static_assert(one_type_less_than_two_type::value == (1 < 2), "Failed");
    typedef decltype(one_type() <= two_type()) one_type_less_than_equal_two_type;
    static_assert(one_type_less_than_equal_two_type::value == (1 <= 2), "Failed");
    typedef decltype(one_type() >= two_type()) one_type_greater_than_equal_two_type;
    static_assert(one_type_greater_than_equal_two_type::value == (1 >= 2), "Failed");
    typedef decltype(one_type() == two_type()) one_type_equal_two_type;
    static_assert(one_type_equal_two_type::value == (1 == 2), "Failed");
    typedef decltype(one_type() != two_type()) one_type_not_equal_two_type;
    static_assert(one_type_not_equal_two_type::value == (1 != 2), "Failed");
    typedef decltype(one_type() & two_type()) one_type_bit_and_two_type;
    static_assert(one_type_bit_and_two_type::value == (1 & 2), "Failed");
    typedef decltype(one_type() ^ two_type()) one_type_xor_two_type;
    static_assert(one_type_xor_two_type::value == (1 ^ 2), "Failed");
    typedef decltype(one_type() | two_type()) one_type_bit_or_two_type;
    static_assert(one_type_bit_or_two_type::value == (1 | 2), "Failed");

};

TICK_STATIC_TEST_CASE()
{
    struct true_type
    : tick::integral_constant<bool, true>
    {};
    static_assert(true_type(), "Failed");
    struct false_type
    : tick::integral_constant<bool, false>
    {};
    static_assert(!false_type(), "Failed");

    typedef tick::integral_constant<int, 1> one_type;
    typedef tick::integral_constant<int, 2> two_type;

    typedef decltype(!true_type()) not_true_type;
    static_assert(not_true_type() == false, "Failed");

    typedef decltype(true_type() && false_type()) true_type_and_false_type;
    static_assert(true_type_and_false_type() == false, "Failed");
    typedef decltype(true_type() || false_type()) true_type_or_false_type;
    static_assert(true_type_or_false_type() == true, "Failed");

    typedef decltype(!two_type()) not_two_type;
    static_assert(not_two_type() == (!2), "Failed");

// GCC confuses the complement operator with the destructor
#ifdef __clang__
    typedef decltype(~two_type()) compl_two_type;
    static_assert(compl_two_type() == (~2), "Failed");
#endif

    typedef decltype(+two_type()) unary_plus_two_type;
    static_assert(unary_plus_two_type() == (+2), "Failed");
    typedef decltype(-two_type()) unary_subtract_two_type;
    static_assert(unary_subtract_two_type() == (-2), "Failed");

    typedef decltype(one_type() + two_type()) one_type_add_two_type;
    static_assert(one_type_add_two_type() == (1 + 2), "Failed");
    typedef decltype(one_type() - two_type()) one_type_subtract_two_type;
    static_assert(one_type_subtract_two_type() == (1 - 2), "Failed");
    typedef decltype(one_type() * two_type()) one_type_multiply_two_type;
    static_assert(one_type_multiply_two_type() == (1 * 2), "Failed");
    typedef decltype(one_type() / two_type()) one_type_divide_two_type;
    static_assert(one_type_divide_two_type() == (1 / 2), "Failed");
    typedef decltype(one_type() % two_type()) one_type_remainder_two_type;
    static_assert(one_type_remainder_two_type() == (1 % 2), "Failed");
    typedef decltype(one_type() >> two_type()) one_type_shift_right_two_type;
    static_assert(one_type_shift_right_two_type() == (1 >> 2), "Failed");
    typedef decltype(one_type() << two_type()) one_type_shift_left_two_type;
    static_assert(one_type_shift_left_two_type() == (1 << 2), "Failed");
    typedef decltype(one_type() > two_type()) one_type_greater_than_two_type;
    static_assert(one_type_greater_than_two_type() == (1 > 2), "Failed");
    typedef decltype(one_type() < two_type()) one_type_less_than_two_type;
    static_assert(one_type_less_than_two_type() == (1 < 2), "Failed");
    typedef decltype(one_type() <= two_type()) one_type_less_than_equal_two_type;
    static_assert(one_type_less_than_equal_two_type() == (1 <= 2), "Failed");
    typedef decltype(one_type() >= two_type()) one_type_greater_than_equal_two_type;
    static_assert(one_type_greater_than_equal_two_type() == (1 >= 2), "Failed");
    typedef decltype(one_type() == two_type()) one_type_equal_two_type;
    static_assert(one_type_equal_two_type() == (1 == 2), "Failed");
    typedef decltype(one_type() != two_type()) one_type_not_equal_two_type;
    static_assert(one_type_not_equal_two_type() == (1 != 2), "Failed");
    typedef decltype(one_type() & two_type()) one_type_bit_and_two_type;
    static_assert(one_type_bit_and_two_type() == (1 & 2), "Failed");
    typedef decltype(one_type() ^ two_type()) one_type_xor_two_type;
    static_assert(one_type_xor_two_type() == (1 ^ 2), "Failed");
    typedef decltype(one_type() | two_type()) one_type_bit_or_two_type;
    static_assert(one_type_bit_or_two_type() == (1 | 2), "Failed");

};

TICK_STATIC_TEST_CASE()
{
    struct true_type
    : tick::integral_constant<bool, true>
    {};
    static_assert(true_type(), "Failed");
    struct false_type
    : tick::integral_constant<bool, false>
    {};
    static_assert(!false_type(), "Failed");

    typedef tick::integral_constant<int, 1> one_type;
    typedef tick::integral_constant<int, 2> two_type;

    static_assert((!true_type()) == false, "Failed");

    static_assert((true_type() && false_type()) == false, "Failed");
    static_assert((true_type() || false_type()) == true, "Failed");

    static_assert((!two_type()) == (!2), "Failed");

// GCC confuses the complement operator with the destructor
#ifdef __clang__
    static_assert((~two_type()) == (~2), "Failed");
#endif

    static_assert((+two_type()) == (+2), "Failed");
    static_assert((-two_type()) == (-2), "Failed");

    static_assert((one_type() + two_type()) == (1 + 2), "Failed");
    static_assert((one_type() - two_type()) == (1 - 2), "Failed");
    static_assert((one_type() * two_type()) == (1 * 2), "Failed");
    static_assert((one_type() / two_type()) == (1 / 2), "Failed");
    static_assert((one_type() % two_type()) == (1 % 2), "Failed");
    static_assert((one_type() >> two_type()) == (1 >> 2), "Failed");
    static_assert((one_type() << two_type()) == (1 << 2), "Failed");
    static_assert((one_type() > two_type()) == (1 > 2), "Failed");
    static_assert((one_type() < two_type()) == (1 < 2), "Failed");
    static_assert((one_type() <= two_type()) == (1 <= 2), "Failed");
    static_assert((one_type() >= two_type()) == (1 >= 2), "Failed");
    static_assert((one_type() == two_type()) == (1 == 2), "Failed");
    static_assert((one_type() != two_type()) == (1 != 2), "Failed");
    static_assert((one_type() & two_type()) == (1 & 2), "Failed");
    static_assert((one_type() ^ two_type()) == (1 ^ 2), "Failed");
    static_assert((one_type() | two_type()) == (1 | 2), "Failed");

};

TICK_STATIC_TEST_CASE()
{
    typedef tick::integral_constant<int, 1> one_type;
    typedef std::integral_constant<int, 1> one_type_std;

    typedef decltype(one_type() == one_type_std()) result_type;
    static_assert(result_type::value, "Failed");
};
