
#include "test.h"
#include <tick/trait_check.h>

TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x++)
    >;
};

TICK_TRAIT(is_integer_incrementable, is_incrementable<_>, std::is_integral<_>)
{
};

#define TRAIT_CHECK_SUCCESS(...) \
static_assert(std::is_same<typename tick::detail::trait_check_impl<__VA_ARGS__>::type, tick::TRAIT_CHECK_FAILURE<>>::value, "Trait check failure")

#define TRAIT_CHECK_FAIL(...) \
static_assert(not std::is_same<typename tick::detail::trait_check_impl<__VA_ARGS__>::type, tick::TRAIT_CHECK_FAILURE<>>::value, "Trait check failure")

TICK_STATIC_TEST_CASE()
{
    TICK_TRAIT_CHECK(is_incrementable<int>, is_incrementable<int*>);
    TICK_TRAIT_CHECK(is_integer_incrementable<int>);

    TRAIT_CHECK_SUCCESS(is_incrementable<int>, is_incrementable<int*>);
    TRAIT_CHECK_SUCCESS(is_integer_incrementable<int>);

    TRAIT_CHECK_FAIL(is_integer_incrementable<int*>, is_integer_incrementable<float>);
};
