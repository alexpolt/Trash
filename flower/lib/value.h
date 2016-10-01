#pragma once

#include "macros.h"
#include <new>


namespace lib {
  
  $t<$n T0> struct value {

    $t<$n U0 = T0, $n... TT> 
    static auto create( TT&&... args ) {

      $static_assert( sizeof( U0 ) <= 2 * sizeof( void* ) );
      $static_assert( alignof( void* ) == alignof( U0 ) );

      auto v = value< T0 >{};

      new( v.data ) U0{ args... };

      return v;

    }

    T0* operator->() {
      
      return (T0*) data;

    }

    alignas( alignof( void* ) ) unsigned char data[ 2 * sizeof( void* ) ];

  };


}

