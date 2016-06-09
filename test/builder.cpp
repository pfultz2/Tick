
#include "test.h"
#include <tick/builder.h>

#ifdef _MSC_VER
template<class T>
constexpr bool as_bool()
{
    return T{};
}
#else
template<class T>
constexpr T as_bool()
{
    return {};
}
#endif

TICK_STATIC_TEST_CASE()
{
    struct foo_int
    {
        typedef int type;
    };

    struct foo_void
    {
        typedef void type;
    };

    TICK_TRAIT(has_int_type)
    {
        template<class T>
        auto require(const T&) -> valid<
            has_type<typename T::type, int>
        >;
    };

    static_assert(as_bool<has_int_type<foo_int>>(), "Failed has_type");
    static_assert(!as_bool<has_int_type<foo_void>>(), "Failed has_type");
};

TICK_STATIC_TEST_CASE()
{

    TICK_TRAIT(is_simple_incrementable)
    {
        template<class T>
        auto require(T&& x) -> valid<
            decltype(returns<T>(x++))
        >;
    };

    static_assert(is_simple_incrementable<int>::value, "Type is not incrementable");

};

TICK_STATIC_TEST_CASE()
{

    TICK_TRAIT(has_foo_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            TICK_RETURNS(x.foo(), int)
        >;
    };

    TICK_TRAIT(has_more_foo_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            is_true<has_foo_member<T>>
        >;
    };

    TICK_TRAIT(has_integral_foo_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            TICK_RETURNS(x.foo(), std::is_integral<_>)
        >;
    };

    TICK_TRAIT(has_simple_foo_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            decltype(x.foo())
        >;
    };

    TICK_TRAIT(has_void_foo_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            TICK_RETURNS(x.foo(), void)
        >;
    };

    // TODO: test base traits
    struct foo_member
    {
        int foo();
    };

    struct long_foo_member
    {
        long foo();
    };

    struct no_foo_member
    {};

    struct invalid_foo_member
    {
        struct invalid
        {};

        invalid foo();
    };

    struct void_foo_member
    {
        void foo();
    };

    template<template<class...> class HasFoo, bool IsSimple>
    struct test_foo_member
    {
        typedef decltype(HasFoo<foo_member>()) check_has_foo_member;
        static_assert(as_bool<HasFoo<foo_member>>(), "No foo member");
        static_assert(as_bool<HasFoo<foo_member>>(), "No foo member");
        static_assert(as_bool<HasFoo<long_foo_member>>(), "No foo member");
        static_assert(not as_bool<HasFoo<no_foo_member>>(), "Foo member found");
        static_assert(as_bool<HasFoo<invalid_foo_member>>() == IsSimple, "Invalid foo member found");
        static_assert(as_bool<HasFoo<void_foo_member>>() == IsSimple, "Invalid void foo member found");
    };

    TICK_TEST_TEMPLATE(test_foo_member<has_foo_member, false>);
    TICK_TEST_TEMPLATE(test_foo_member<has_more_foo_member, false>);
    TICK_TEST_TEMPLATE(test_foo_member<has_integral_foo_member, false>);
    TICK_TEST_TEMPLATE(test_foo_member<has_simple_foo_member, true>);
    TICK_TEST_TEMPLATE(test_foo_member<has_void_foo_member, true>);

};

TICK_STATIC_TEST_CASE()
{

    TICK_TRAIT(has_foo_bar_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            TICK_RETURNS(x.foo(), int),
            TICK_RETURNS(x.bar(), int)
        >;
    };

    TICK_TRAIT(has_more_foo_bar_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            is_true<has_foo_bar_member<T>>
        >;
    };

    TICK_TRAIT(has_integral_foo_bar_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            TICK_RETURNS(x.foo(), std::is_integral<_>),
            TICK_RETURNS(x.bar(), std::is_integral<_>)
        >;
    };

    TICK_TRAIT(has_simple_foo_bar_member)
    {
        template<class T>
        auto require(T&& x) -> valid<
            decltype(x.foo()),
            decltype(x.bar())
        >;
    };
    // TODO: test base traits
    struct foo_bar_member
    {
        int foo();
        int bar();
    };

    struct long_foo_bar_member
    {
        long foo();
        long bar();
    };

    struct no_foo_bar_member
    {};

    struct no_bar_member
    {
        void foo();
    };

    struct invalid_foo_bar_member
    {
        struct invalid
        {};

        invalid foo();
        invalid bar();
    };

    struct void_foo_bar_member
    {
        void foo();
        void bar();
    };

    template<template<class...> class HasFoo, bool IsSimple>
    struct test_foo_bar_member
    {
        static_assert(as_bool<HasFoo<foo_bar_member>>(), "No foo, bar member");
        static_assert(as_bool<HasFoo<long_foo_bar_member>>(), "No foo, bar member");
        static_assert(not as_bool<HasFoo<no_foo_bar_member>>(), "Foo, bar member found");
        static_assert(not as_bool<HasFoo<no_bar_member>>(), "Bar member found");
        static_assert(as_bool<HasFoo<invalid_foo_bar_member>>() == IsSimple, "Invalid foo, bar member found");
        static_assert(as_bool<HasFoo<void_foo_bar_member>>() == IsSimple, "Invalid void foo, bar member found");
    };

    TICK_TEST_TEMPLATE(test_foo_bar_member<has_foo_bar_member, false>);
    TICK_TEST_TEMPLATE(test_foo_bar_member<has_more_foo_bar_member, false>);
    TICK_TEST_TEMPLATE(test_foo_bar_member<has_integral_foo_bar_member, false>);
    TICK_TEST_TEMPLATE(test_foo_bar_member<has_simple_foo_bar_member, true>);

};

namespace wtf {
    struct left_comma {};
    template<class T> T operator,(T, left_comma);

    struct right_comma {};
    template<class T> T operator,(right_comma, T);
}


TICK_STATIC_TEST_CASE()
{
    struct comma_guard
    {
        struct any
        {
            template<class T>
            any(T&&);
        };
        const comma_guard& operator,(any) const;
        // friend const comma_guard& operator,(any, comma_guard);
        // friend const comma_guard& operator,(comma_guard, any);
    };

    // Disable this test for comma_guard since it fails
    // typedef typename tick::detail::bare<decltype(comma_guard(), ((void)0), wtf::left_comma(), 0)>::type comma_guard_result;
    // STATIC_ASSERT_SAME(comma_guard_result, comma_guard);


    TICK_TRAIT(has_funs)
    {
        template<class T>
        auto require(T&& x) -> valid<
            decltype(x.f1()),
            decltype(x.f2()),
            decltype(x.f3())
        >;
    };

    template<class T, class U>
    struct f1_T
    {
        T f1();
        U f2();
        U f3();
    };

    template<class T, class U>
    struct f2_T
    {
        U f1();
        T f2();
        U f3();
    };

    template<class T, class U>
    struct void_f1
    {
        void f1();
        T f2();
        U f3();
    };

    struct no_funs {};

    template<template<class...> class F>
    struct test_funs
    {
        struct some_type {};
        struct noncopyable_type
        {
            noncopyable_type(const noncopyable_type&) = delete;
            noncopyable_type(noncopyable_type&) = delete;
            noncopyable_type(noncopyable_type&&) = delete;
        };
        static_assert(has_funs<F<int, int>>::value, "No funs");
        static_assert(has_funs<F<void, int>>::value, "No funs");
        static_assert(has_funs<F<int, void>>::value, "No funs");

        static_assert(has_funs<F<wtf::left_comma, some_type>>::value, "No funs");
        static_assert(has_funs<F<wtf::right_comma, some_type>>::value, "No funs");

        static_assert(has_funs<F<wtf::left_comma, noncopyable_type>>::value, "No funs");
        static_assert(has_funs<F<wtf::right_comma, noncopyable_type>>::value, "No funs");

        static_assert(has_funs<F<wtf::left_comma, void>>::value, "No funs");
        static_assert(has_funs<F<wtf::right_comma, void>>::value, "No funs");

    };


    static_assert(!has_funs<no_funs>::value, "Found funs");
    TICK_TEST_TEMPLATE(test_funs<f1_T>);
    TICK_TEST_TEMPLATE(test_funs<f2_T>);
    TICK_TEST_TEMPLATE(test_funs<void_f1>);
};

TICK_STATIC_TEST_CASE()
{
    TICK_TRAIT(has_nested_type)
    {
        template<class T>
        auto require(T) -> valid<
            has_type<typename T::type, int>
        >;
    };

    TICK_TRAIT(has_integral_nested_type)
    {
        template<class T>
        auto require(T) -> valid<
            has_type<typename T::type, std::is_integral<_>>
        >;
    };

    TICK_TRAIT(has_simple_nested_type)
    {
        template<class T>
        auto require(T) -> valid<
            has_type<typename T::type>
        >;
    };

    struct nested_type
    {
        typedef int type;
    };

    struct no_nested_type
    {
        typedef int type_;
    };

    struct invalid_nested_type
    {
        struct invalid {};
        typedef invalid type;
    };

    struct void_nested_type
    {
        typedef void type;
    };

    struct template_nested_type
    {
        template<class T>
        struct type {};
    };

    static_assert(has_nested_type<nested_type>(), "No nested type");
    static_assert(not has_nested_type<no_nested_type>(), "nested type found");
    static_assert(not has_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(not has_nested_type<invalid_nested_type>(), "Templated nested type found");
    static_assert(not has_nested_type<void_nested_type>(), "Invalid void nested type found");
    static_assert(not has_nested_type<template_nested_type>(), "Templated nested type found");

    static_assert(has_integral_nested_type<nested_type>(), "No nested type");
    static_assert(not has_integral_nested_type<no_nested_type>(), "nested type found");
    static_assert(not has_integral_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(not has_integral_nested_type<invalid_nested_type>(), "Templated nested type found");
    static_assert(not has_integral_nested_type<void_nested_type>(), "Invalid void nested type found");
    static_assert(not has_integral_nested_type<template_nested_type>(), "Templated nested type found");

    static_assert(has_simple_nested_type<nested_type>(), "No nested type");
    static_assert(not has_simple_nested_type<no_nested_type>(), "nested type found");
    static_assert(has_simple_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(has_simple_nested_type<void_nested_type>(), "No void nested type found");
    static_assert(not has_simple_nested_type<template_nested_type>(), "Templated nested type found");

};

TICK_STATIC_TEST_CASE()
{
    TICK_TRAIT(has_nested_type)
    {
        template<class T>
        auto require(T) -> valid<
            decltype(has_type<typename T::type, int>())
        >;
    };

    TICK_TRAIT(has_integral_nested_type)
    {
        template<class T>
        auto require(T) -> valid<
            decltype(has_type<typename T::type, std::is_integral<_>>())
        >;
    };

    TICK_TRAIT(has_simple_nested_type)
    {
        template<class T>
        auto require(T) -> valid<
            decltype(has_type<typename T::type>())
        >;
    };

    struct nested_type
    {
        typedef int type;
    };

    struct no_nested_type
    {
        typedef int type_;
    };

    struct invalid_nested_type
    {
        struct invalid {};
        typedef invalid type;
    };

    struct void_nested_type
    {
        typedef void type;
    };

    struct template_nested_type
    {
        template<class T>
        struct type {};
    };

    static_assert(has_nested_type<nested_type>(), "No nested type");
    static_assert(not has_nested_type<no_nested_type>(), "nested type found");
    static_assert(not has_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(not has_nested_type<invalid_nested_type>(), "Templated nested type found");
    static_assert(not has_nested_type<void_nested_type>(), "Invalid void nested type found");
    static_assert(not has_nested_type<template_nested_type>(), "Templated nested type found");

    static_assert(has_integral_nested_type<nested_type>(), "No nested type");
    static_assert(not has_integral_nested_type<no_nested_type>(), "nested type found");
    static_assert(not has_integral_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(not has_integral_nested_type<invalid_nested_type>(), "Templated nested type found");
    static_assert(not has_integral_nested_type<void_nested_type>(), "Invalid void nested type found");
    static_assert(not has_integral_nested_type<template_nested_type>(), "Templated nested type found");

    static_assert(has_simple_nested_type<nested_type>(), "No nested type");
    static_assert(not has_simple_nested_type<no_nested_type>(), "nested type found");
    static_assert(has_simple_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(has_simple_nested_type<void_nested_type>(), "No void nested type found");
    static_assert(not has_simple_nested_type<template_nested_type>(), "Templated nested type found");

};


TICK_STATIC_TEST_CASE()
{
    TICK_TRAIT(has_nested_type)
    {
        template<class T>
        auto require(T) -> decltype(
            has_type<typename T::type, int>()
        );
    };

    TICK_TRAIT(has_integral_nested_type)
    {
        template<class T>
        auto require(T) -> decltype(
            has_type<typename T::type, std::is_integral<_>>()
        );
    };

    TICK_TRAIT(has_numberic_nested_type)
    {
        template<class T>
        auto require(T) -> decltype(
            has_type<typename T::type, std::is_integral<_>, std::is_arithmetic<_>>()
        );
    };

    TICK_TRAIT(has_simple_nested_type)
    {
        template<class T>
        auto require(T) -> decltype(
            has_type<typename T::type>()
        );
    };

    struct nested_type
    {
        typedef int type;
    };

    struct no_nested_type
    {
        typedef int type_;
    };

    struct invalid_nested_type
    {
        struct invalid {};
        typedef invalid type;
    };

    struct floating_point_nested_type
    {
        typedef float type;
    };

    struct void_nested_type
    {
        typedef void type;
    };

    struct template_nested_type
    {
        template<class T>
        struct type {};
    };

    static_assert(has_nested_type<nested_type>(), "No nested type");
    static_assert(not has_nested_type<no_nested_type>(), "nested type found");
    static_assert(not has_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(has_nested_type<floating_point_nested_type>(), "Floating point nested type not found");
    static_assert(not has_nested_type<void_nested_type>(), "Invalid void nested type found");
    static_assert(not has_nested_type<template_nested_type>(), "Templated nested type found");

    static_assert(has_integral_nested_type<nested_type>(), "No nested type");
    static_assert(not has_integral_nested_type<no_nested_type>(), "nested type found");
    static_assert(not has_integral_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(not has_integral_nested_type<floating_point_nested_type>(), "Floating point nested type found");
    static_assert(not has_integral_nested_type<void_nested_type>(), "Invalid void nested type found");
    static_assert(not has_integral_nested_type<template_nested_type>(), "Templated nested type found");

    static_assert(has_numberic_nested_type<nested_type>(), "No nested type");
    static_assert(not has_numberic_nested_type<no_nested_type>(), "nested type found");
    static_assert(not has_numberic_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(not has_numberic_nested_type<floating_point_nested_type>(), "Floating point nested type found");
    static_assert(not has_numberic_nested_type<void_nested_type>(), "Invalid void nested type found");
    static_assert(not has_numberic_nested_type<template_nested_type>(), "Templated nested type found");

    static_assert(has_simple_nested_type<nested_type>(), "No nested type");
    static_assert(not has_simple_nested_type<no_nested_type>(), "nested type found");
    static_assert(has_simple_nested_type<invalid_nested_type>(), "Invalid nested type found");
    static_assert(has_simple_nested_type<floating_point_nested_type>(), "Floating point nested type found");
    static_assert(has_simple_nested_type<void_nested_type>(), "No void nested type found");
    static_assert(not has_simple_nested_type<template_nested_type>(), "Templated nested type found");

};

TICK_STATIC_TEST_CASE()
{
    struct nested_template
    {
        template<class T>
        struct template_
        {};
    };

    struct no_nested_template
    {
    };

    struct invalid_nested_template
    {
        struct template_ {};
    };

    TICK_TRAIT(has_nested_template)
    {
        template<class T>
        auto require(T) -> valid<
            has_template<T::template template_>
        >;
    };

    static_assert(has_nested_template<nested_template>(), "No nested template");
    static_assert(not has_nested_template<no_nested_template>(), "nested template found");
    static_assert(not has_nested_template<invalid_nested_template>(), "Invalid nested template found");
};

TICK_STATIC_TEST_CASE()
{
    struct nested_template
    {
        template<class T>
        struct template_
        {};
    };

    struct no_nested_template
    {
    };

    struct invalid_nested_template
    {
        struct template_ {};
    };

    TICK_TRAIT(has_nested_template)
    {
        template<class T>
        auto require(T) -> decltype(
            has_template<T::template template_>()
        );
    };

    static_assert(has_nested_template<nested_template>(), "No nested template");
    static_assert(not has_nested_template<no_nested_template>(), "nested template found");
    static_assert(not has_nested_template<invalid_nested_template>(), "Invalid nested template found");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_integer)
    {
        template<class T>
        auto require(T) -> valid<
            is_true<std::is_integral<T>>
        >;
    };

    static_assert(is_integer<int>(), "is_integer predicate failed");
    static_assert(not is_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_integer)
    {
        template<class T>
        auto require(T) -> valid<
            is_true_c<std::is_integral<T>::value>
        >;
    };

    static_assert(is_integer<int>(), "is_integer predicate failed");
    static_assert(not is_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_integer)
    {
        template<class T>
        auto require(T) -> decltype(
            is_true<std::is_integral<T>>()
        );
    };

    static_assert(is_integer<int>(), "is_integer predicate failed");
    static_assert(not is_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_integer)
    {
        template<class T>
        auto require(T) -> decltype(
            is_true_c<std::is_integral<T>::value>()
        );

    };
    static_assert(is_integer<int>(), "is_integer predicate failed");
    static_assert(not is_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_not_integer)
    {
        template<class T>
        auto require(T) -> valid<
            is_false<std::is_integral<T>>
        >;

    };
    static_assert(not is_not_integer<int>(), "is_integer predicate failed");
    static_assert(is_not_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_not_integer)
    {
        template<class T>
        auto require(T) -> valid<
            is_false_c<std::is_integral<T>::value>
        >;

    };
    static_assert(not is_not_integer<int>(), "is_integer predicate failed");
    static_assert(is_not_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_not_integer)
    {
        template<class T>
        auto require(T) -> decltype(
            is_false<std::is_integral<T>>()
        );

    };
    static_assert(not is_not_integer<int>(), "is_integer predicate failed");
    static_assert(is_not_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{

    struct no_is_integer
    {};

    TICK_TRAIT(is_not_integer)
    {
        template<class T>
        auto require(T) -> decltype(
            is_false_c<std::is_integral<T>::value>()
        );

    };
    static_assert(not is_not_integer<int>(), "is_integer predicate failed");
    static_assert(is_not_integer<no_is_integer>(), "is_integer predicate failed");
};

TICK_STATIC_TEST_CASE()
{
    struct no_is_integer
    {};

    TICK_TRAIT(is_integer, std::is_integral<_>)
    {};

    static_assert(is_integer<int>(), "is_integer refinement failed");
    static_assert(not is_integer<no_is_integer>(), "is_integer refinement failed");
};

TICK_STATIC_TEST_CASE()
{
    struct no_is_integer
    {};

    TICK_TRAIT(is_integer, quote<std::is_integral>)
    {};

    static_assert(is_integer<int>(), "is_integer refinement failed");
    static_assert(not is_integer<no_is_integer>(), "is_integer refinement failed");
};

