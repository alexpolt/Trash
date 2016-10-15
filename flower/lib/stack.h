#pragma once

#include "macros.h"
#include "vector.h"

namespace lib {

  TP<TN T0, TN T1 = vector< T0 >>
  struct stack : T1 {

    using value_type = T0;

    void push( value_type value ) { $this.push_back( move( value ) ); }

    value_type pop() { return $this.pop_back(); }

  };

}

