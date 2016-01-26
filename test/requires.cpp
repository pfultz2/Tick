
#include "test.h"
#include <type_traits>
#include <tick/integral_constant.h>
#include <tick/requires.h>
#include <cassert>

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

TICK_STATIC_TEST_CASE()
{
    template<class T, TICK_REQUIRES(std::is_integral<T>::value)>
    static std::true_type check_requires(T);

    template<class T, TICK_REQUIRES(!std::is_integral<T>::value)>
    static std::true_type check_requires(T);

    STATIC_ASSERT_SAME(decltype(check_requires(1)), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_requires(not_int())), std::true_type);
};

TICK_STATIC_TEST_CASE()
{
    template<class T, class... Ts, TICK_REQUIRES(std::is_integral<T>::value)>
    static std::true_type check_vardiac_requires(T, Ts...);

    template<class T, class... Ts, TICK_REQUIRES(!std::is_integral<T>::value)>
    static std::false_type check_vardiac_requires(T, Ts...);

    STATIC_ASSERT_SAME(decltype(check_vardiac_requires(1)), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_vardiac_requires(not_int())), std::false_type);
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

TICK_STATIC_TEST_CASE()
{
    template<class T>
    struct check_member_requires
    {
        T x;

        TICK_MEMBER_REQUIRES(std::is_integral<T>::value)
        std::true_type foo()
        {
            x += 1;
            return std::true_type();
        }

        TICK_MEMBER_REQUIRES(!std::is_integral<T>::value)
        std::false_type foo()
        {
            return std::false_type();
        }
    };

    STATIC_ASSERT_SAME(decltype(check_member_requires<int>().foo()), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_member_requires<not_int>().foo()), std::false_type);
};

TICK_STATIC_TEST_CASE()
{
    template<class T>
    struct check_member_requires
    {
        T x;

        TICK_MEMBER_REQUIRES(std::is_integral<T>::value)
        int foo()
        {
            x += 1;
            return x;
        }

        TICK_MEMBER_REQUIRES(!std::is_integral<T>::value)
        int foo()
        {
            return 0;
        }
    };

    STATIC_ASSERT_SAME(decltype(check_member_requires<int>().foo()), int);
    STATIC_ASSERT_SAME(decltype(check_member_requires<not_int>().foo()), int);
};

template<class T>
struct is_integral
: tick::integral_constant<bool, std::is_integral<T>::value>
{};

template<class T>
std::true_type check_param_requires(T, TICK_PARAM_REQUIRES(is_integral<T>()));

template<class T>
std::false_type check_param_requires(T, TICK_PARAM_REQUIRES(!is_integral<T>()));

TICK_STATIC_TEST_CASE()
{
    STATIC_ASSERT_SAME(decltype(check_param_requires(1)), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_param_requires(not_int())), std::false_type);
};

TICK_STATIC_TEST_CASE()
{
    static const int x = 1;
    typedef decltype(tick::trait<std::is_integral>(x)) test_type;
    static_assert(test_type::value, "Trait test failed");
};

template<class T>
std::true_type check_param_trait_requires(T x, TICK_PARAM_REQUIRES(tick::trait<std::is_integral>(x)));

template<class T>
std::false_type check_param_trait_requires(T x, TICK_PARAM_REQUIRES(!tick::trait<std::is_integral>(x)));

TICK_STATIC_TEST_CASE()
{
    STATIC_ASSERT_SAME(decltype(check_param_trait_requires(1)), std::true_type);
    STATIC_ASSERT_SAME(decltype(check_param_trait_requires(not_int())), std::false_type);
};



template<class T>
struct member_requires_oc
{
    T x;

    TICK_MEMBER_REQUIRES(std::is_same<T, int>::value)
    int foo();

    TICK_MEMBER_REQUIRES(std::is_same<T, char>::value)
    char foo();
};

template<class T>
TICK_MEMBER_REQUIRES_OC(std::is_same<T, int>::value)
int member_requires_oc<T>::foo()
{
    return 123;
}

template<class T>
TICK_MEMBER_REQUIRES_OC(std::is_same<T, char>::value)
char member_requires_oc<T>::foo()
{
    return 'a';
}

TICK_TEST_CASE()
{
    assert(member_requires_oc<int>().foo() == 123);
    assert(member_requires_oc<char>().foo() == 'a');
}
