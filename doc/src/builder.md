Build traits using the TICK_TRAIT macro
=======================================

This macro will build a boolean type trait for you. Each trait requires a `requires` member function of the form:
```cpp
TICK_TRAIT(my_trait)
{
    template<class T>
    auto requires_(T&& x) -> decltype(
        ...
    );
};
```
The parameters to the trait are based on the parameters to the `requires` function. Then inside the `decltype`, all the expressions are placed that will be check for. If one of the expressions is not valid, the the trait will return false. 

Refinements
-----------

Refinements can be expressed after the name. Each refinement is a [placeholder expression](http://www.boost.org/doc/libs/1_55_0/libs/mpl/doc/refmanual/placeholder-expression.html), where each placeholder(ie `_1`, `_2`, etc) are replaced by their corresponding type passed into the trait. In the case of traits that accept a single parameter the unnamed placeholder(`_`) can be used, for example:
```cpp
TICK_TRAIT(is_incrementable, std::is_default_constructible<_>)
{
    template<class T>
    auto requires_(T&& x) -> decltype(
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
    auto requires_(T&& x, U&& y) -> decltype(
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
    auto requires_(T&& x) -> decltype(
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
    auto requires_(T&& x) -> decltype(
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
    auto requires_(T&& x) -> decltype(
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
    auto requires_(const T& x) -> decltype(
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
    auto requires_(const T& x) -> decltype(
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
    auto requires_(const T& x) -> decltype(
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
    auto requires_(const T& x) -> decltype(
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
    auto requires_(T&& x) -> decltype(
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
    auto requires_(T&& x) -> decltype(
        x++,
        ++x
    );
};
```
Notice, the placeholders have to be fully qualified here.

