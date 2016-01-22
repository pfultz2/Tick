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

