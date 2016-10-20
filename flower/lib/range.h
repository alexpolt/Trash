#pragma once

#include "macros.h"
#include "types.h"

namespace lib {

  TP<TN T0> struct range_t {
  
    struct iterator {
      constexpr operator T0&() { return _index; }
      constexpr T0& operator *() { return _index; }
      T0 _index;
    };

    constexpr iterator begin() const { return iterator{ _begin }; }

    constexpr iterator end() const { return iterator{ _end }; }

    constexpr T0 size() const { return _end - _begin; }

    T0 _begin;
    T0 _end;

  };

  using range = range_t< ssize_t >;

  TP<TN T0, TN T1>
  int operator+( range_t< T0 > rng, T1 fn ) {

    for( ssize_t i = rng.begin(), end = rng.end(); i != end; i++ ) fn();

    return 0;
  }

  #define $do + [&]()

}

