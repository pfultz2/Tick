
#include "test.h"
#include <tick/traits.h>
#include <tick/trait_check.h>

#include <memory>
#include <map>
#include <list>
#include <set>

TICK_STATIC_TEST_CASE()
{

    TICK_TRAIT_CHECK(tick::is_equality_comparable<int>);
    TICK_TRAIT_CHECK(tick::is_weakly_ordered<int>);
    TICK_TRAIT_CHECK(tick::is_totally_ordered<int>);

    TICK_TRAIT_CHECK(tick::is_equality_comparable<int*>);
    TICK_TRAIT_CHECK(tick::is_weakly_ordered<int*>);
    TICK_TRAIT_CHECK(tick::is_totally_ordered<int*>);

    TICK_TRAIT_CHECK(tick::is_swappable<int>);
    TICK_TRAIT_CHECK(tick::is_swappable<int*>);

    TICK_TRAIT_CHECK(tick::is_nullable_pointer<int*>);
    TICK_TRAIT_CHECK(tick::is_nullable_pointer<std::shared_ptr<int>>);

    TICK_TRAIT_CHECK(tick::is_input_iterator<std::istream_iterator<char>>);
    TICK_TRAIT_CHECK(tick::is_iterator<std::insert_iterator<std::vector<int>>>);
    TICK_TRAIT_CHECK(tick::is_output_iterator<std::insert_iterator<std::vector<int>>, int>);

    TICK_TRAIT_CHECK(tick::is_iterator<std::list<int>::const_iterator>);
    TICK_TRAIT_CHECK(tick::is_forward_iterator<std::list<int>::const_iterator>);
    TICK_TRAIT_CHECK(tick::is_bidirectional_iterator<std::list<int>::const_iterator>);
    TICK_TRAIT_CHECK(tick::is_mutable_bidirectional_iterator<std::list<int>::iterator>);
    static_assert(not tick::is_mutable_bidirectional_iterator<std::list<int>::const_iterator>(), "Const iterator mutable");
    static_assert(not tick::is_mutable_random_access_iterator<std::list<int>::const_iterator>(), "List const iterator is mutable and has random access");
    static_assert(not tick::is_random_access_iterator<std::list<int>::iterator>(), "List iterator has random access");

    TICK_TRAIT_CHECK(tick::is_random_access_iterator<std::vector<int>::const_iterator>);
    TICK_TRAIT_CHECK(tick::is_mutable_random_access_iterator<std::vector<int>::iterator>);
    static_assert(not tick::is_mutable_random_access_iterator<std::vector<int>::const_iterator>(), "Const iterator mutable");

    TICK_TRAIT_CHECK(tick::is_value_swappable<std::vector<int>::iterator>);
    TICK_TRAIT_CHECK(tick::is_value_swappable<std::list<int>::iterator>);
};

TICK_STATIC_TEST_CASE()
{
    TICK_TRAIT_CHECK(tick::is_range<std::vector<int>>);
    TICK_TRAIT_CHECK(tick::is_range<std::list<int>>);
    TICK_TRAIT_CHECK(tick::is_range<std::set<int>>);
    TICK_TRAIT_CHECK(tick::is_range<std::map<int, int>>);


    TICK_TRAIT_CHECK(tick::is_container<std::vector<int>>);
    TICK_TRAIT_CHECK(tick::is_container<std::list<int>>);
    TICK_TRAIT_CHECK(tick::is_container<std::set<int>>);
    TICK_TRAIT_CHECK(tick::is_container<std::map<int, int>>);

    TICK_TRAIT_CHECK(tick::is_reversible_container<std::vector<int>>);
    TICK_TRAIT_CHECK(tick::is_reversible_container<std::list<int>>);
    TICK_TRAIT_CHECK(tick::is_reversible_container<std::set<int>>);
    TICK_TRAIT_CHECK(tick::is_reversible_container<std::map<int, int>>);

    TICK_TRAIT_CHECK(tick::is_sequence_container<std::vector<int>>);
    TICK_TRAIT_CHECK(tick::is_sequence_container<std::list<int>>);

    static_assert(!tick::is_sequence_container<std::map<int, int>>(), "Not a sequence container");
    static_assert(!tick::is_sequence_container<std::set<int>>(), "Not a sequence container");
};

