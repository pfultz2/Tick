Building traits
===============

TICK_TRAIT
----------

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

This will essentially build a class that inherits from `integral_constant`, so the above is equivalent to this:

```cpp
template<class... Ts>
struct my_trait
: integral_constant<bool, (...)>
{};
```
The parameters to the trait are based on the parameters passed to the `require` function. 

The trait will be either true or false if the expressions given are valid. Each expression in `valid` needs a `decltype` around it. If one of the expressions is not valid, the the trait will return false. For example,
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

In addition `quote` can be used to pass all the args from the trait to the refinement:

```cpp
TICK_TRAIT(is_comparable, 
    quote<is_equality_comparable>)
{
    template<class T, class U>
    auto require(T&& x, U&& y) -> valid<
        decltype(x < y),
        decltype(x <= y),
        decltype(x >= y),
        decltype(x > y)
    >;
};
```

Query operations
================

These can be used to query more information about the types then just valid expressions.

When a type is matched in query operation, it can either be convertible to the type given, or the evaluated [placeholder expression](http://www.boost.org/doc/libs/1_55_0/libs/mpl/doc/refmanual/placeholder-expression.html) must be true. Placeholder expressions can be given so the type can be matched against other traits.


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

Also, `returns<void>` is prohibited.

```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(returns<void>(x++)) // Compiler error
    >;
};
```

Instead, use either `decltype` directly without `returns`, or if there is a possibility of `void` from a computed type, use `TICK_RETURNS` or `has_type` instead.

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

is_true
-------

The `is_true` query can check if a trait is true. Using [refinements](refinements) is the preferred way of checking for additional traits, but this can be useful if the evaluation of some trait can't be used lazily with [placeholder expression](http://www.boost.org/doc/libs/1_55_0/libs/mpl/doc/refmanual/placeholder-expression.html). So `is_true` can be used instead, for example:
```cpp
TICK_TRAIT(is_2d_array)
{
    template<class T>
    auto require(const T& x) -> valid<
        is_true<std::is_same<std::rank<T>::type, std::integral_constant<std::size_t, 2>> >
    >;
};
```

is_false
--------

The `is_false` query can check if a trait is false. Using [refinements](refinements) is the preferred way of checking for additional traits, but this can be useful if the evaluation of some trait can't be used lazily with [placeholder expression](http://www.boost.org/doc/libs/1_55_0/libs/mpl/doc/refmanual/placeholder-expression.html). So `is_false` can be used instead, for example:
```cpp
TICK_TRAIT(is_multi_array)
{
    template<class T>
    auto require(const T& x) -> valid<
        is_false<std::is_same<std::rank<T>::type, std::integral_constant<std::size_t, 1>> >
    >;
};
```

as_const
--------

The `as_const` function helps ensure that lvalues are `const`.

```cpp
TICK_TRAIT(is_copy_assignable)
{
    template<class T>
    auto require(T&& x) -> valid<
        decltype(x = as_const(x))
    >;
};
```

as_mutable
----------

The `as_mutable` function helps ensure that lvalues are treated as `mutable` or non-const.

```cpp
TICK_TRAIT(is_destructible)
{
    template<class T>
    auto require(const T& x) -> valid<
        decltype(as_mutable(x).~T())
    >;
};
```

Build traits without macros
===========================

The traits can be built without using the `TICK_TRAIT` macros. However, it may introduce problems with portability. So if only one platform is needed to be supported, then here's how to build them. First, build a class for the `require` functions and inherit from `tick::ops` to bring in all the query operations:
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
: tick::models<is_incrementable_r, Ts...>
{};
```

refines
-------

Refinements can be used by using the `refines` class:
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

