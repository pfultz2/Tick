Tick
====

Trait introspection and concept creator for C++11

Getting Started
===============

Tick provides a mechanism for easily defining and using traits in C++11. For example, if we defined a generic `increment` function, like this:
```cpp
template<class T>
void increment(T& x)
{
    x++;
}
```
If we pass something that does not have the `++` operator to `increment`, we will get an error inside of the `increment` function. This can make it unclear whether the error is due to a mistake by the user of the function or by the implementor of the function. Instead we want to check the type requirements of the function. 

Using Tick we can create an `is_incrementable` trait, like this:
```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x++),
        decltype(++x)
    >;
};
```
And then we can use a simple requires clause in our function to check the type requirements:
```cpp
template<class T, TICK_REQUIRES(is_incrementable<T>())>
void increment(T& x)
{
    x++;
}
```
So, now, if we pass something that is not incrementable to `increment`:
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

This gives an error at the call to `increment` rather than inside the function, and then pointes to the type requirements of the function. This gives enough information for most commons cases, however, sometimes we may want more information. In that case the `TICK_TRAIT_CHECK` can be used. For example, say we had the `is_incrementable` trait defined like this:
```cpp
TICK_TRAIT(is_incrementable, std::is_integral<_>)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x++),
        decltype(++x)
    >;
};
```
Then if we use `TICK_TRAIT_CHECK`, we can see why `int*` is not incrementable:
```cpp
TICK_TRAIT_CHECK(is_incrementable<int*>);
```
Which will produce this error:

    ../tick/trait_check.h:95:38: error: implicit instantiation of undefined template 'tick::TRAIT_CHECK_FAILURE<std::is_integral<int *>, is_incrementable<int *> >'

Which shows the traits that failed including any refinements. So we can see that it failed because `std::is_integral<int *>` is not true.

Build traits using the TICK_TRAIT macro
=======================================

This macro will build a boolean type trait for you. Each trait has a `require` member function of the form:
```cpp
TICK_TRAIT(my_trait)
{
    template<class T>
    auto require(T&& x) -> valid<
        ...
    >;
};
```
The parameters to the trait are based on the parameters passed to the `require` function. Then inside the `valid`, all the expressions are placed that will be check for. Each expression in `valid` needs a `decltype` around it. If one of the expressions is not valid, the the trait will return false. For example,
```cpp
TICK_TRAIT(my_trait)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x++)
    >;
};
```
The trait above will check that `x++` is a valid expression.

Refinements
-----------

Refinements can be expressed after the name. Each refinement is a [placeholder expression](http://www.boost.org/doc/libs/1_55_0/libs/mpl/doc/refmanual/placeholder-expression.html), where each placeholder(ie `_1`, `_2`, etc) are replaced by their corresponding type passed into the trait. In the case of traits that accept a single parameter the unnamed placeholder(`_`) can be used, for example:
```cpp
TICK_TRAIT(is_incrementable, std::is_default_constructible<_>)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x++),
        decltype(++x)
    >;
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
    auto require(T&& x, U&& y) -> valid<
        decltype(x == y),
        decltype(x != y)
    >;
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
    auto require(T&& x) -> valid<
        decltype(returns<int>(x++))
    >;
};
```
This trait will be true if the expressions `x++` is valid and is convertible to `int`.

Here's an example using placeholder expressions as well:
```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(returns<std::is_integral<_>>(x++))
    >;
};
```
This trait will be true if the expressions `x++` is valid and returns a type that `is_integral`.

Note: The `TICK_RETURNS` macro can be used instead to improve compatability with older compilers(such as gcc 4.6):
```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto require(T&& x) -> valid<
        TICK_RETURNS(x++, int)
    >;
};
```

has_type
--------

The `has_type` query can check if a type exist and if the type matches. For example:
```cpp
TICK_TRAIT(has_nested_type)
{
    template<class T>
    auto require(const T& x) -> valid<
        has_type<typename T::type>
    >;
};
```
This trait will be true if `T` has a nested type called `type`.

Now `has_type` used as above is not quite as useful since the above example, can also be simply written without `has_type` like this:
```cpp
TICK_TRAIT(has_nested_type)
{
    template<class T>
    auto require(const T& x) -> valid<
        typename T::type
    >;
};
```
So, an optional second parameter can be provided to check if the type matches. Here's an example:
```cpp
TICK_TRAIT(has_nested_int_type)
{
    template<class T>
    auto require(const T& x) -> valid<
        has_type<typename T::type, std::is_integral<_>>
    >;
};
```
This trait will be true if `T` has a nested type called `type` which is an integral type.

Note: For older compilers such as gcc 4.6 the `has_type` has to be used inside of a `decltype`, like this:
```cpp
TICK_TRAIT(has_nested_int_type)
{
    template<class T>
    auto require(const T& x) -> valid<
        decltype(has_type<typename T::type, std::is_integral<_>>())
    >;
};
```
Also, a `TICK_HAS_TYPE` macro is provided as well, which takes care of wrapping it in a `decltype`:
```cpp
TICK_TRAIT(has_nested_int_type)
{
    template<class T>
    auto require(const T& x) -> valid<
        TICK_HAS_TYPE(T::type, std::is_integral<_>>)
    >;
};
```

has_template
------------

The `has_template` query can check if a template exist. For example:
```cpp
TICK_TRAIT(has_nested_result)
{
    template<class T>
    auto require(const T& x) -> valid<
        has_template<T::template result>
    >;
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
    auto require(const T& x) -> valid<
        is_true<std::is_same<std::rank<T>::type, std::integral_constant<std::size_t, 2>> >
    >;
};
```

Note: For older compilers such as gcc 4.6 the `is_true` has to be used inside of a `decltype`, like this:
```cpp
TICK_TRAIT(is_2d_array)
{
    template<class T>
    auto require(const T& x) -> valid<
        decltype(is_true<std::is_same<std::rank<T>::type, std::integral_constant<std::size_t, 2>> >())
    >;
};
```
Also the macros `TICK_IS_TRUE` and `TICK_IS_FALSE` are provided as well,  which takes care of wrapping it in a `decltype`:
```cpp
TICK_TRAIT(is_2d_array)
{
    template<class T>
    auto require(const T& x) -> valid<
        TICK_IS_TRUE(std::is_same<std::rank<T>::type, std::integral_constant<std::size_t, 2>> >)
    >;
};
```

Build traits without macros
===========================

The traits can be built using the `TICK_TRAIT` macros. Heres how to build them. First, build a class for the `require` functions and inherit from `tick::ops` to bring in all the query operations:
```cpp
struct is_incrementable_r : tick::ops
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x++),
        decltype(++x)
    >;
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
    auto require(T&& x) -> valid<
        decltype(x++),
        decltype(++x)
    >;
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

TICK_MEMBER_REQUIRES
--------------------

The `TICK_MEMBER_REQUIRES` can be used for member function inside of classes, that are not templated. For example,
```cpp
template<class T>
struct foo
{
    T x;

    TICK_MEMBER_REQUIRES(is_incrementable<T>())
    void up()
    {
        x++;
    }
};
```

If the member is defined outside of the class, use `TICK_MEMBER_REQUIRES_OC` (OC: outside_class) outside the class. For example,
```cpp
template<class T>
struct foo
{
    T x;

    TICK_MEMBER_REQUIRES(is_incrementable<T>())
    void up();
};

template<class T>
TICK_MEMBER_REQUIRES_OC(is_incrementable<T>())
void foo<T>::up()
{
    x++;
}
```

TICK_PARAM_REQUIRES
--------------------

The `TICK_PARAM_REQUIRES` can be used in the paramater of the function. This is useful for lambdas:
```cpp
auto increment = [](auto& x, TICK_PARAM_REQUIRES(is_incrementable<decltype(x)>()))
{
    x++;
};
```
Also, the `trait` function is provided which can be used to deduce the type of the parameters:
```cpp
auto increment = [](auto& x, TICK_PARAM_REQUIRES(trait<is_incrementable>(x)))
{
    x++;
};
```
Note: The `trait` function always deduces the type without references. So `trait<std::is_lvalue_reference>(x)` will always be false.

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

The `TICK_TRAIT_CHECK` macro will statically assert the list of traits that are true but it will show what traits failed including base traits. This can be useful to show more informative messages about why a trait is false.

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
    auto require(T&& x) -> valid<
        decltype(x++),
        decltype(++x)
    >;
};
```
Acknowledgments
===============

* Eric Niebler for the idea of using a `requires` member function to check concept traits.[http://ericniebler.com/2013/11/23/concept-checking-in-c11/]
* Jamboree for the idea of using a template class to place expressions.[https://github.com/ericniebler/range-v3/issues/29#issuecomment-51016277] 

