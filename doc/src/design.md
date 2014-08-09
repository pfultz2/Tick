Design Notes
============

Using template class
--------------------

Tick uses a the `tick::valid` template class to place valid expressions, because it provides a more robust solution. Ideally, using tick we could define the traits like this:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            x++,
            ++x
        );
    };

However, if one of the expressions returns a type that overloads the comma operator in a strange way(rare but still possible), then the trait could fail(ie return false when it should be true). To fix it, we could add `void` casts like this:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(
            (void)x++,
            (void)++x
        );
    };

However, the `void` casts can be easy to forget. Another solution to the problem could be to pass it to a function:

    template<class... Ts>
    void valid_expr(T&&...);

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(valid_expr(
            x++,
            ++x
        ));
    };

However, if one of the expressions returns `void`, then this will fail as well(ie return false when it should be true). So this could be fixed in a similiar way as well:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto requires_(T&& x) -> decltype(valid_expr(
            (x++, 1),
            (++x, 1)
        ));
    };

However, it can be easy to forget to put the `1` in there. So instead we use a `tick::valid` template class, like this:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto requires_(T&& x) -> tick::valid<
            decltype(x++),
            decltype(++x)
        >;
    };

This requires placing each expression in a decltype, but if this was forgotten there would be a compile error pointing to the incorrect expression.

Trait-based
-----------

The concept predicates in Tick are defined as regular type traits(ie they are integrel constants) instead of a `constexpr bool` function. They are almost functionally the same in use. However, as a trait, it allows for better flexibility and expressiveness, through higher-order programming. This is what enables passing the traits to other functions which can be used to match return types to traits as well as other types.

No tag-dispatching
------------------

Currently, Tick does not support tag-dispatching for overloading functions. That is because tag-dispatching doesn't work well with specialization. So instead of tag-dispatching, conditional overloading can be used. Here is example of implementing `advance` using the [`conditional_adaptor`](http://pfultz2.github.io/Fit/doc/html/conditional/) from the Fit library:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto requires_(T&& x) -> tick::valid<
            decltype(x++),
            decltype(++x)
        >;
    };

    TICK_TRAIT(is_advanceable, is_incrementable<_>)
    {
        template<class T, class I>
        auto requires_(T&& x, I&& i) -> tick::valid<
            decltype(x += i)
        >;
    };

    struct advance_advanceable
    {
        template<class Iterator, TICK_REQUIRES(is_advanceable<Iterator, int>())>
        void operator()(Iterator& it, int n) const
        {
            it += n;
        }
    };

    struct advance_incrementable
    {
        template<class Iterator, TICK_REQUIRES(is_incrementable<Iterator>())>
        void operator()(Iterator& it, int n) const
        {
            while (n--) ++it;
        }
    };

    static fit::static_<fit::conditional_adaptor<advance_advanceable, advance_incrementable>> advance = {};

So if the `advance_advanceable` function is not callable then it will try to call the `advance_incrementable` function.
