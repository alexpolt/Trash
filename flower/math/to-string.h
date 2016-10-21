#pragma once

#include "../lib/macros.h"
#include "../lib/types.h"
#include "../lib/error.h"

#include "types.h"

namespace lib {

  TP<TN T0, ssize_t... NN > 
  cstr to_string( m::vec_t< T0, NN... > value ) {

    //info, "v0 = "; for( auto i : v0.data() ) info, i, ", "; info, endl, endl;
    $throw $error_not_implemented();
  }



  TP<TN T0, ssize_t... NN > 
  cstr to_string( m::mat_t< T0, NN... > value ) {
    //info, "mat.v0 = "; for( auto i : m0[0].data() ) info, i, ", "; info, endl;
    //info, "mat.v1 = "; for( auto i : m0[1].data() ) info, i, ", "; info, endl;
    //info, "mat.v2 = "; for( auto i : m0[2].data() ) info, i, ", "; info, endl;
    $throw $error_not_implemented();
  }


}


