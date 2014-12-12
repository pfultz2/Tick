
#include "test.h"
#include <tick/tag.h>
#include <list>
#include <vector>

TICK_TRAIT(is_incrementable)
{
    template<class T>
    auto requires_(T&& x) -> tick::valid<
        decltype(x++),
        decltype(++x)
    >;
};

TICK_TRAIT(is_decrementable, is_incrementable<_>)
{
    template<class T>
    auto requires_(T&& x) -> tick::valid<
        decltype(x--),
        decltype(--x)
    >;
};

TICK_TRAIT(is_advanceable, is_decrementable<_>)
{
    template<class T, class Number>
    auto requires_(T&& x, Number n) -> tick::valid<
        decltype(x += n)
    >;
};

TICK_STATIC_TEST_CASE()
{
    typedef typename is_advanceable<tick::detail::no_check>::type::tick_trait_refinements check_refines;
    typedef typename tick::detail::get_refinements<is_advanceable<int>>::type advanceable_refinements;
    static_assert(std::is_same<check_refines, advanceable_refinements>(), "Refinements don't match");
    static_assert(not std::is_same<advanceable_refinements, tick::refines<>>(), "Empty refinements");
};


TICK_STATIC_TEST_CASE()
{
    static_assert(std::is_base_of<tick::tag<is_decrementable>, tick::tag<is_advanceable>>(), "Not tag base");
    static_assert(std::is_base_of<tick::tag<is_incrementable>, tick::tag<is_advanceable>>(), "Not tag base");
    static_assert(std::is_base_of<tick::tag<is_incrementable>, tick::tag<is_decrementable>>(), "Not tag base");
};

TICK_STATIC_TEST_CASE()
{
    static_assert(is_advanceable<int, int>(), "Not advanceable");
    static_assert(is_decrementable<int>(), "Not decrementable");
    static_assert(is_incrementable<int>(), "Not incrementable");

    static_assert(std::is_base_of<tick::tag<is_advanceable>, tick::most_refined<is_advanceable<int, int>>>(), "Not tag base");
    static_assert(std::is_base_of<tick::tag<is_decrementable>, tick::most_refined<is_advanceable<int, int>>>(), "Not tag base");
    static_assert(std::is_base_of<tick::tag<is_incrementable>, tick::most_refined<is_advanceable<int, int>>>(), "Not tag base");

    static_assert(not std::is_base_of<tick::tag<is_advanceable>, tick::most_refined<is_decrementable<int>>>(), "Not tag base");
    static_assert(std::is_base_of<tick::tag<is_decrementable>, tick::most_refined<is_decrementable<int>>>(), "Not tag base");
    static_assert(std::is_base_of<tick::tag<is_incrementable>, tick::most_refined<is_decrementable<int>>>(), "Not tag base");

    static_assert(not std::is_base_of<tick::tag<is_advanceable>, tick::most_refined<is_incrementable<int>>>(), "Not tag base");
    static_assert(not std::is_base_of<tick::tag<is_decrementable>, tick::most_refined<is_incrementable<int>>>(), "Not tag base");
    static_assert(std::is_base_of<tick::tag<is_incrementable>, tick::most_refined<is_incrementable<int>>>(), "Not tag base");
};

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

TICK_TEST_CASE()
{
    std::list<int> l = { 1, 2, 3, 4, 5, 6 };
    auto iterator = l.begin();
    advance(iterator, 4);
    TICK_TEST_CHECK(*iterator == 5);
}

TICK_TEST_CASE()
{
    std::list<int> l = { 1, 2, 3, 4, 5, 6 };
    auto iterator = l.end();
    advance(iterator, -4);
    TICK_TEST_CHECK(*iterator == 3);
}

TICK_TEST_CASE()
{
    std::vector<int> v = { 1, 2, 3, 4, 5, 6 };
    auto iterator = v.begin();
    advance(iterator, 4);
    TICK_TEST_CHECK(*iterator == 5);
}


