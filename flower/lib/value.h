#pragma once

#include "macros.h"
#include <new>


namespace lib {
  
  $t<$n T0> struct value {

    $t<$n U0 = T0, $n... TT> 
    static auto create( TT&&... args ) {

      $static_assert( sizeof( U0 ) <= sizeof( data ) );
      $static_assert( alignof( T0 ) == alignof( U0 ) );

      auto v = value< T0 >{};

      new( v.data ) U0{ args... };

      return v;

    }

    $t<$n... TT>
    auto operator()( TT... args ) {

      return ( (T0&) data )( args... );

    }

    T0* operator->() {

      return (T0*) data;
      
    }

    using byte = unsigned char;

    alignas( alignof( T0 ) ) byte data[ sizeof( void*[2] ) ];

  };


}

