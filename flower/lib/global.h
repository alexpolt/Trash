#pragma once

#include "macros.h"
#include "types.h"
#include "vector.h"


namespace global {

  using namespace lib;

  TP<TN T0 = char, ssize_t N0 = 256 > 
  auto& get_buffer() { 

    thread_local static T0 array[ N0 ];

    return array; 
  }


}


