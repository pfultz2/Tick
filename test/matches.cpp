

#include "test.h"
#include <tick/detail/matches.h>

template<class... T>
struct replace_args_test
{};

TICK_TEST_CASE()
{
    STATIC_ASSERT_SAME(typename tick::detail::replace_args<replace_args_test<int>, long>::type, 
        replace_args_test<int>);
    STATIC_ASSERT_SAME(typename tick::detail::replace_args<replace_args_test<tick::_>, long>::type, 
        replace_args_test<long>);
    STATIC_ASSERT_SAME(typename tick::detail::replace_args<replace_args_test<tick::_1>, long>::type, 
        replace_args_test<long>);
    STATIC_ASSERT_SAME(typename tick::detail::replace_args<replace_args_test<tick::_1, tick::_2>, long, float>::type, 
        replace_args_test<long, float>);
    STATIC_ASSERT_SAME(typename tick::detail::replace_args<tick::quote<replace_args_test>, long>::type, 
        replace_args_test<long>);
    STATIC_ASSERT_SAME(typename tick::detail::replace_args<tick::local_quote::quote<replace_args_test>, long>::type, 
        replace_args_test<long>);
}

TICK_TEST_CASE()
{
    static_assert(tick::detail::matches<int, int>::value, "int -> int doesn't match");
    static_assert(tick::detail::matches<int, float>::value, "int -> float doesn't match");
    static_assert(tick::detail::matches<int, void>::value, "int -> void doesn't match");
    static_assert(tick::detail::matches<void, void>::value, "void -> void doesn't match");

    static_assert(tick::detail::matches<std::is_integral<tick::_>, int>::value, "is_integral<int> failed");
    static_assert(tick::detail::matches<std::is_integral<tick::_>, float>::value, "is_integral<float> failed");
    static_assert(tick::detail::matches<std::is_integral<tick::_>, void>::value, "is_integral<void> failed");
}
