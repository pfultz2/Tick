
#include "test.h"
#include <tick/builder.h>

TICK_STATIC_TEST_CASE()
{

    TICK_TRAIT(has_foo_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            TICK_RETURNS(x.foo(), int)
        );
    };

    TICK_TRAIT(has_more_foo_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            is_true<has_foo_member<T>>()
        );
    };

    TICK_TRAIT(has_integral_foo_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            TICK_RETURNS(x.foo(), std::is_integral<_>)
        );
    };

    TICK_TRAIT(has_simple_foo_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            x.foo()
        );
    };
    // TICK_TRAIT(has_void_foo_member)
    // {
    //     template<class T>
    //     auto requires_(T&& x) -> decltype(
    //         returns<void>(x.foo())
    //         // TICK_RETURNS(x.foo(), void)
    //     );
    // };
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
        static_assert(HasFoo<foo_member>(), "No foo member");
        static_assert(HasFoo<long_foo_member>(), "No foo member");
        static_assert(not HasFoo<no_foo_member>(), "Foo member found");
        static_assert(HasFoo<invalid_foo_member>() == IsSimple, "Invalid foo member found");
        static_assert(HasFoo<void_foo_member>() == IsSimple, "Invalid void foo member found");
    };

    TICK_TEST_TEMPLATE(test_foo_member<has_foo_member, false>);
    TICK_TEST_TEMPLATE(test_foo_member<has_more_foo_member, false>);
    TICK_TEST_TEMPLATE(test_foo_member<has_integral_foo_member, false>);
    TICK_TEST_TEMPLATE(test_foo_member<has_simple_foo_member, true>);
    // TICK_TEST_TEMPLATE(test_foo_member<has_void_foo_member, true>);

};

TICK_STATIC_TEST_CASE()
{

    TICK_TRAIT(has_foo_bar_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            TICK_RETURNS(x.foo(), int),
            TICK_RETURNS(x.bar(), int)
        );
    };

    TICK_TRAIT(has_more_foo_bar_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            is_true<has_foo_bar_member<T>>()
        );
    };

    TICK_TRAIT(has_integral_foo_bar_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            TICK_RETURNS(x.foo(), std::is_integral<_>),
            TICK_RETURNS(x.bar(), std::is_integral<_>)
        );
    };

    TICK_TRAIT(has_simple_foo_bar_member)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            x.foo(),
            x.bar()
        );
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
        static_assert(HasFoo<foo_bar_member>(), "No foo, bar member");
        static_assert(HasFoo<long_foo_bar_member>(), "No foo, bar member");
        static_assert(not HasFoo<no_foo_bar_member>(), "Foo, bar member found");
        static_assert(not HasFoo<no_bar_member>(), "Bar member found");
        static_assert(HasFoo<invalid_foo_bar_member>() == IsSimple, "Invalid foo, bar member found");
        static_assert(HasFoo<void_foo_bar_member>() == IsSimple, "Invalid void foo, bar member found");
    };

    TICK_TEST_TEMPLATE(test_foo_bar_member<has_foo_bar_member, false>);
    TICK_TEST_TEMPLATE(test_foo_bar_member<has_more_foo_bar_member, false>);
    TICK_TEST_TEMPLATE(test_foo_bar_member<has_integral_foo_bar_member, false>);
    TICK_TEST_TEMPLATE(test_foo_bar_member<has_simple_foo_bar_member, true>);

};

TICK_STATIC_TEST_CASE()
{
    TICK_TRAIT(has_nested_type)
    {
        template<class T>
        auto requires_(T) -> decltype(
            has_type<typename T::type, int>()
        );
    };

    TICK_TRAIT(has_integral_nested_type)
    {
        template<class T>
        auto requires_(T) -> decltype(
            has_type<typename T::type, std::is_integral<_>>()
        );
    };

    TICK_TRAIT(has_simple_nested_type)
    {
        template<class T>
        auto requires_(T) -> decltype(
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
        auto requires_(T) -> decltype(
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
        auto requires_(T) -> decltype(
            is_true<std::is_integral<T>>()
        );
    };

    static_assert(is_integer<int>(), "is_integer predicate failed");
    static_assert(not is_integer<no_is_integer>(), "is_integer predicate failed");
};

