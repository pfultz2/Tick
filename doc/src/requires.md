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
