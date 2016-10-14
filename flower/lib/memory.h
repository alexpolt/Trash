#pragma once

#include <cstdlib>
#include "macros.h"
#include "error.h"
#include "types.h"

namespace lib {

  struct error_memory : error {

    using error::error;

  };


  $T<$N T0>
  T0* alloc( ssize_t size ) {

    auto ptr = (T0*) ::malloc( size );

    if( ! ptr ) throw error_memory{ "malloc failed" };

    return ptr;

  };

  $T<$N T0>
  void free( T0*& ptr, ssize_t size ) { 
    
    ::free( ptr ); 
    
    ptr = nullptr; 
    
  };


}

