#pragma once

#include "macros.h"
#include "types.h"

namespace lib {

  TP<TN T0> struct range_t {
  
    struct iterator {
      operator T0&() { return _index; }
      T0& operator *() { return _index; }
      T0 _index;
    };

    iterator begin() const { return iterator{ _begin }; }
    iterator end() const { return iterator{ _end }; }

    T0 _begin;
    T0 _end;

  };

  using range = range_t< ssize_t >;

}

