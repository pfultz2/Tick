
#include "test.h"
#include <tick/detail/fold.h>
#include <type_traits>

template<class... Ts>
struct list {};

template<int... Ns>
using list_c = list<std::integral_constant<int, Ns>...>;

TICK_STATIC_TEST_CASE()
{
    typedef list<long,float,short,double,float,long,long double> types;

    template<class State, class Elem>
    struct op
    : std::conditional<std::is_floating_point<Elem>::value,
        std::integral_constant<int, State::value + 1>,
        State
    >
    {};

    typedef tick::detail::fold<
          types, 
          std::integral_constant<int, 0>, 
          op
        >::type number_of_floats;

    static_assert(number_of_floats::value == 4, "Fold test failed");
};
