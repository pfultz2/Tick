Tag dispatching
===============

Tag dispatching allows for functions to be ordered by the refinements defined in the trait. For example, if we try to implement an `advance` function like [`std::advance`](http://en.cppreference.com/w/cpp/iterator/advance). First, we can define the traits for the different traversals:

```cpp
TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto require(T&& x) -> tick::valid<
        decltype(x++),
        decltype(++x)
    >;
};

TICK_TRAIT(is_decrementable, is_incrementable<_>)
{
    template<class T>
    auto require(T&& x) -> tick::valid<
        decltype(x--),
        decltype(--x)
    >;
};

TICK_TRAIT(is_advanceable, is_decrementable<_>)
{
    template<class T, class Number>
    auto require(T&& x, Number n) -> tick::valid<
        decltype(x += n)
    >;
};
```

Then we can try to use template constraints for the different overloads:

```cpp
template<class Iterator, TICK_REQUIRES(is_advanceable<Iterator>())>
void advance(Iterator& it, int n)
{
    it += n;
}

template<class Iterator, TICK_REQUIRES(is_decrementable<Iterator>())>
void advance(Iterator& it, int n)
{
    if (n > 0) while (n--) ++it;
    else 
    {
        n *= -1;
        while (n--) --it;
    }
}

template<class Iterator, TICK_REQUIRES(is_incrementable<Iterator>())>
void advance(Iterator& it, int n)
{
    while (n--) ++it;
}
```

However, this leads to ambiguities when we try to use it with iterators to vectors. That is because those iterators are valid for all three overloads. So, tag dispatching allows us to pick the overload that is the most refined. First, we need to call `most_refined` which will retrieve the tags for each each trait. So then `advance` could be implemented like this:

```cpp
template<class Iterator>
void advance_impl(Iterator& it, int n, tick::tag<is_advanceable>)
{
    it += n;
}

template<class Iterator>
void advance_impl(Iterator& it, int n, tick::tag<is_decrementable>)
{
    if (n > 0) while (n--) ++it;
    else 
    {
        n *= -1;
        while (n--) --it;
    }
}

template<class Iterator>
void advance_impl(Iterator& it, int n, tick::tag<is_incrementable>)
{
    while (n--) ++it;
}

template<class Iterator>
void advance(Iterator& it, int n)
{
    advance_impl(it, n, tick::most_refined<is_advanceable<Iterator>>());
}
```

Specialization
--------------

Tag dispatching will still work with specialization. Say, for instance, someone implemented an iterator called `foo_iterator` that when the user called `+=` it would crash at runtime. So we would like to specialize `is_advanceable` to make it false, so the `+=` won't be called:

```cpp
template<>
struct is_advanceable<foo_iterator>
: std::false_type
{};
```

So this will exclude the `is_advanceable` overload, but the `is_decrementable` and `is_incrementable` will still be called.
