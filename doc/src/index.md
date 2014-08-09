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
    auto requires_(T&& x) -> tick::valid<
        decltype(x++),
        decltype(++x)
    >;
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
        auto requires_(T&& x) -> tick::valid<
            decltype(x++),
            decltype(++x)
        >;
    };

Then if we use `TICK_TRAIT_CHECK`, we can see why `int*` is not incrementable:

    TICK_TRAIT_CHECK(is_incrementable<int*>);

Which will produce this error:

    ../tick/trait_check.h:95:38: error: implicit instantiation of undefined template 'tick::TRAIT_CHECK_FAILURE<std::is_integral<int *>, is_incrementable<int *> >'

Which shows the traits that failed including any refinements. So we can see that it failed because `std::is_integral<int *>` is not true.

