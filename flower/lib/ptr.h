#pragma once

#include "macros.h"

namespace lib {

  $T<$N T0>
  struct raw_ptr {

    raw_ptr( T0* ptr ) : ptr{ ptr } { }

    T0* operator->() {
      return ptr;
    }

    T0 const * operator->() const {
      return ptr;
    }

    T0& operator*() {
      return *ptr;
    }

    T0 const& operator*() const {
      return *ptr;
    }

    T0* ptr;
  };

}


