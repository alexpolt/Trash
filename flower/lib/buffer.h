#pragma once

#include "macros.h"
#include "types.h"

namespace lib {

  namespace global {

    TP<TN T0, ssize_t N0 = 256 > 
    inline auto& get_buffer() { 

      thread_local static char array[ N0 ];

      return array; 
    }


  }


}




