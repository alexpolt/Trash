#pragma once

#include "macros.h"

namespace lib {

  $T<$N T0>
  struct raw_ptr {

    raw_ptr( T0* _ptr ) : _ptr{ _ptr } { }

    T0* operator->() { return _ptr; }

    T0 const * operator->() const { return _ptr; }

    T0& operator*() { return *_ptr; }

    T0 const& operator*() const { return *_ptr; }

    T0* _ptr;
  };

}


