#pragma once

#include "vector.h"

namespace lib {


  using string = vector< char, 0, true >;


  TP<TN T0, ssize_t N0>
  auto make_string( T0 ( &data)[ N0 ] ) {
    
    return vector< char, N0, true >{ data };
  }



}

