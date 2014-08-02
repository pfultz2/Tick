
#include "test.h"
#include <type_traits>
#include <tick/integral_constant.h>
#include <tick/requires.h>

struct not_int {};

TICK_STATIC_TEST_CASE()
{
    template<class T, TICK_REQUIRES(std::is_integral<T>::value)>
    static std::true_type check_requires(T);

    template<class T, TICK_REQUIRES(!std::is_integral<T>::value)>
    static std::false_type check_requires(T);

    STATIC_ASSERT_SAME(decltype(check_requires(1)), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_requires(not_int())), std::false_type);
};

template<class T> 
TICK_FUNCTION_REQUIRES(std::is_integral<T>::value)
(std::true_type) check_function_requires(T);

template<class T> 
TICK_FUNCTION_REQUIRES(!std::is_integral<T>::value)
(std::false_type) check_function_requires(T);

TICK_STATIC_TEST_CASE()
{
    STATIC_ASSERT_SAME(decltype(check_function_requires(1)), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_function_requires(not_int())), std::false_type);
};

TICK_STATIC_TEST_CASE()
{
    template<class T, class=void>
    struct check_class_requires;

    template<class T>
    struct check_class_requires<T, TICK_CLASS_REQUIRES(std::is_integral<T>::value)>
    {
        static std::true_type call();
    };

    template<class T>
    struct check_class_requires<T, TICK_CLASS_REQUIRES(!std::is_integral<T>::value)>
    {
        static std::false_type call();
    };

    STATIC_ASSERT_SAME(decltype(check_class_requires<int>::call()), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_class_requires<not_int>::call()), std::false_type);
};