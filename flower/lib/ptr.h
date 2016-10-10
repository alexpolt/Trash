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


  $T<$N T0>
  struct out {

    explicit out( T0& value ) : _value{ value } { }

    

    T0& _value;
  };

}


