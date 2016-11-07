
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

