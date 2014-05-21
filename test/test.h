#ifndef GUARD_TEST_H
#define GUARD_TEST_H

#include <type_traits>
#include <tick/detail/pp.h>

#define STATIC_ASSERT_SAME(...) static_assert(std::is_same<__VA_ARGS__>::value, "Types are not the same")
#define TICK_TEST_CASE() void TICK_PP_CAT(test_, __LINE__) ()
#define TICK_STATIC_TEST_CASE() struct TICK_PP_CAT(test_, __LINE__)

#define TICK_TEST_TEMPLATE(...) typedef std::integral_constant<int, sizeof(__VA_ARGS__)> TICK_PP_CAT(test_template_, __LINE__)

int main()
{
    return 0;
}
 
#endif