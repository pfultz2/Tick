Tick
====

Trait introspection and concept creator for C++11

Getting Started
===============

Tick provides a mechanism for easily defining and using traits in C++11. It is based on ideas developed by Eric Niebler in [this blog post](http://ericniebler.com/2013/11/23/concept-checking-in-c11/).

For example we could create an `is_incrementable` trait, like this:
```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        x++,
        ++x
    );
};
```
And then we can use a simple requires clause in our functions:
```cpp
template<class T, TICK_REQUIRES(is_incrementable<T>())>
void increment(T& x)
{
    x++;
}
```
So if we pass something that is not incrementable to `increment`:
```cpp
struct foo {};

foo f;
increment(f);
```
Then we get an error like this in clang:

    demo.cpp:25:2: error: no matching function for call to 'increment'
            increment(f);
            ^~~~~~~~~
    demo.cpp:14:19: note: candidate template ignored: disabled by 'enable_if' [with T = foo]
    template<class T, TICK_REQUIRES(is_incrementable<T>())>
                      ^

This is pretty concise and gives enough information for most commons cases, however, sometimes we may want more information. In that case the `TICK_TRAIT_CHECK` can be used. For example, say we had a `is_incrementable` type defined like this:

    TICK_TRAIT(is_incrementable, std::is_integral<_>)
    {
        template<class T>
        auto requires_(T&& x) -> TICK_VALID(
            x++,
            ++x
        );
    };

Then if we use `TICK_TRAIT_CHECK`, we can see why `int*` is not incrementable:

    TICK_TRAIT_CHECK(is_incrementable<int*>);

Which will produce this error:

    ../tick/trait_check.h:95:38: error: implicit instantiation of undefined template 'tick::TRAIT_CHECK_FAILURE<std::is_integral<int *>, is_incrementable<int *> >'

Which shows the traits that failed including any refinements. So we can see that it failed because `std::is_integral<int *>` is not true.


Build traits using the TICK_TRAIT macro
=======================================

This macro will build a boolean type trait for you. Each trait requires a `requires` member function of the form:
```cpp
TICK_TRAIT(my_trait)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        ...
    );
};
```
The parameters to the trait are based on the parameters to the `requires` function. Then inside the `TICK_VALID` macro, all the expressions are placed that will be check for. If one of the expressions is not valid, the the trait will return false. 

Refinements
-----------

Refinements can be expressed after the name. Each refinement is a [placeholder expression](http://www.boost.org/doc/libs/1_55_0/libs/mpl/doc/refmanual/placeholder-expression.html), where each placeholder(ie `_1`, `_2`, etc) are replaced by their corresponding type passed into the trait. In the case of traits that accept a single parameter the unnamed placeholder(`_`) can be used, for example:
```cpp
TICK_TRAIT(is_incrementable, std::is_default_constructible<_>)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        x++,
        ++x
    );
};
```
This trait will be true when `x++` and `++x` are valid expressions and `x` is default constructible.

When a trait has multiple parameters, its best to use named placeholders. For example:
```cpp
TICK_TRAIT(is_equality_comparable, 
    std::is_default_constructible<_1>, 
    std::is_default_constructible<_2>)
{
    template<class T, class U>
    auto requires_(T&& x, U&& y) -> TICK_VALID(
        x == y,
        x != y
    );
};
```
This trait will be true when `x == y` and `x != y` are valid expressions and both `x` and `y` are default constructible.

Query operations
================

These can be used to query more information about the types then just valid expressions.

Type matching
-------------

When a type is matched, it can either be convertible to the type given, or the evaluated placeholder expression must be true. Placeholder expressions can be given so the type can be matched against other traits.


returns
-------

The `returns` query can check if the result of the expressions matches the type. For example,
```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        returns<int>(x++)
    );
};
```
This trait will be true if the expressions `x++` is valid and is convertible to `int`.

Here's an example using placeholder expressions as well:
```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        returns<std::is_integral<_>>(x++)
    );
};
```
This trait will be true if the expressions `x++` is valid and returns a type that `is_integral`.

Note: The `TICK_RETURNS` macro can be used instead to improve compatability with older compilers(such as gcc 4.6):
```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        TICK_RETURNS(x++, int)
    );
};
```

has_type
--------

The `has_type` query can check if a type exist and if the type matches. For example:
```cpp
TICK_TRAIT(has_nested_type)
{
    template<class T>
    auto requires_(const T& x) -> TICK_VALID(
        has_type<typename T::type>()
    );
};
```
This trait will be true if `T` has a nested type called `type`.

Also, an optionally second parameter can be provided to check if the type matches. Here's an example:
```cpp
TICK_TRAIT(has_nested_int_type)
{
    template<class T>
    auto requires_(const T& x) -> TICK_VALID(
        has_type<typename T::type, std::is_integral<_>>()
    );
};
```
This trait will be true if `T` has a nested type called `type` which is an integral type.

has_template
------------

The `has_template` query can check if a template exist. For example:
```cpp
TICK_TRAIT(has_nested_result)
{
    template<class T>
    auto requires_(const T& x) -> TICK_VALID(
        has_template<T::template result>()
    );
};
```
This trait will be true if `T` has a nested template called `result`.

Trait evaluation
----------------

The `is_true` and `is_false` queries can check if a trait is true or false. Using refinements is the preferred way of checking for additional traits, but this can be useful if the evaluation of some trait can't be used lazily with placeholder expressions. So the `is_true` and `is_false` can be used instead, for example:
```cpp
TICK_TRAIT(is_2d_array)
{
    template<class T>
    auto requires_(const T& x) -> TICK_VALID(
        is_true<std::is_same<std::rank<T>::type, std::integral_constant<std::size_t, 2>> >()
    );
};
```

Build traits without macros
===========================

The traits can be built using the `TICK_TRAIT` macros. Heres how to build them. First, build a class for the `requires` functions and inherit from `tick::ops` to bring in all the query operations:
```cpp
struct is_incrementable_r : tick::ops
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        x++,
        ++x
    );
};
```
Next, turn it into a trait using `tick::models`:
```cpp
template<class... Ts>
struct is_incrementable
: tick::models<is_incrementable_r(Ts...)>
{};
```

Refinements
-----------

Refinements can be used by using the `tick::refines` class:
```cpp
struct is_incrementable_r 
: tick::ops, tick::refines<std::is_default_constructible<tick::_>>
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        x++,
        ++x
    );
};
```
Notice, the placeholders have to be fully qualified here.

Template constraints
====================

Three macros are provided to help improve the readability of template constraints.

TICK_REQUIRES
-------------

The `TICK_REQUIRES` can be used on template parameters. For example,
```cpp
template<class T, TICK_REQUIRES(is_incrementable<T>())>
void increment(T& x)
{
    x++;
}
```

This can also be used with classes.

TICK_CLASS_REQUIRES
-------------------

The `TICK_CLASS_REQUIRES` can be used when template specialization is done on classes. For example,
```cpp
template<class T, class=void>
struct foo
{
    ...
};

template<class T>
struct foo<T, TICK_CLASS_REQUIRES(is_incrementable<T>() and not std::is_integral<T>())>
{
    ...
};

template<class T>
struct foo<T, TICK_CLASS_REQUIRES(std::is_integral<T>())>
{
    ...
};
```

TICK_FUNCTION_REQUIRES
----------------------

The `TICK_FUNCTION_REQUIRES` can be used on functions. This requires placing parenthesis around the return type:
```cpp
template<class T>
TICK_FUNCTION_REQUIRES(is_incrementable<T>())
(void) increment(T& x)
{
    x++;
}
```
Note: The `TICK_REQUIRES` should be preferred.

Trait checking
==============

The `TICK_TRAIT_CHECK` macro will statically assert the list of traits are true but it will show what traits failed including base traits. This can be useful to show more informative messages about why a trait is false.

Requirements
============

This requires a C++11 compiler. There a no third-party dependencies. This has been tested on clang 3.4 and gcc 4.6-4.9.

ZLang support
=============

[ZLang](https://github.com/pfultz2/ZLang) is supported for some of the macros. The macros are in the `tick` namespace. For example,
```cpp
$(trait is_incrementable)
{
    template<class T>
    auto requires_(T&& x) -> TICK_VALID(
        x++,
        ++x
    );
};
```
