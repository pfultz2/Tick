#pragma once

#include <tick/builder.h>

namespace tick {

TICK_TRAIT(
    is_copyable
  , std::is_copy_constructible<_>
  , std::is_copy_assignable<_>
) {
};

TICK_TRAIT(is_equality_comparable) {
  template<class T>
  auto requires_(T&& x) -> valid<
      decltype(returns<bool>(x == x))
    , decltype(returns<bool>(x != x))
  >;
};

TICK_TRAIT(is_weakly_ordered) {
  template<class T>
  auto requires_(T&& x) -> valid<
      decltype(returns<bool>(x < x))
    , decltype(returns<bool>(x > x))
    , decltype(returns<bool>(x <= x))
    , decltype(returns<bool>(x >= x))
  >;
};

TICK_TRAIT(
    is_totally_ordered
  , is_weakly_ordered<_>
  , is_equality_comparable<_>
) {
};

TICK_TRAIT(is_allocatable) {
  template<class T>
  auto requires_(T x) -> valid<
      decltype(returns<T*>(new T))
    , decltype(delete new T)
    , decltype(returns<T*>(new T[1]))
    , decltype(delete [] new T[1])
  >;
};

TICK_TRAIT(
    is_semi_regular
  , std::is_default_constructible<_>
  , std::is_copy_constructible<_>
  , std::is_destructible<_>
  , std::is_copy_assignable<_>
  , is_allocatable<_>
) {
};

TICK_TRAIT(
    is_regular
  , is_semi_regular<_>
  , is_equality_comparable<_>
) {
};

} //end namespace
