Design Notes
============

Using template class
--------------------

Tick uses a the `tick::valid` template class to place valid expressions, because it provides a more robust solution. Ideally, using tick we could define the traits like this:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto require(T&& x) -> decltype(
            x++,
            ++x
        );
    };

However, if one of the expressions returns a type that overloads the comma operator in a strange way(rare but still possible), then the trait could fail(ie return false when it should be true). To fix it, we could add `void` casts like this:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto require(T&& x) -> decltype(
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
        auto require(T&& x) -> decltype(valid_expr(
            x++,
            ++x
        ));
    };

However, if one of the expressions returns `void`, then this will fail as well(ie return false when it should be true). So this could be fixed in a similiar way as well:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto require(T&& x) -> decltype(valid_expr(
            (x++, 1),
            (++x, 1)
        ));
    };

However, it can be easy to forget to put the `1` in there. So instead we use a `tick::valid` template class, like this:

    TICK_TRAIT(is_incrementable)
    {
        template<class T>
        auto require(T&& x) -> tick::valid<
            decltype(x++),
            decltype(++x)
        >;
    };

This requires placing each expression in a decltype, but if this was forgotten there would be a compile error pointing to the incorrect expression.

Trait-based
-----------

The concept predicates in Tick are defined as regular type traits(ie they are integrel constants) instead of a `constexpr bool` function. They are almost functionally the same in use. However, as a trait, it allows for better flexibility and expressiveness, through higher-order programming. This is what enables passing the traits to other functions which can be used to match return types to traits as well as other types.

Specializations
---------------

All the traits created can be specialized by the user. This is very important. Since the definition of traits relies on duck typing, there are times that even though it may quack like a duck it is not a duck. So with specialization the user can clarify the type's capabilities.

