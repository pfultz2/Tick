
#include "test.h"
#include <tick/detail/set.h>
#include <type_traits>

TICK_STATIC_TEST_CASE()
{
    typedef tick::detail::set<int, float, double> set1;
    typedef typename tick::detail::set_insert<set1, float>::type set2;
    STATIC_ASSERT_SAME(set1, set2);
};

TICK_STATIC_TEST_CASE()
{
    typedef tick::detail::set<int, float, double> set1;
    typedef typename tick::detail::set_insert<set1, long>::type set2;
    typedef tick::detail::set<int, float, double, long> set3;
    STATIC_ASSERT_SAME(set3, set2);
};
