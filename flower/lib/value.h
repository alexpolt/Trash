#pragma once

#include "macros.h"
#include <new>


namespace lib {
  
  $t<$n T0> struct value {

    $t<$n U0 = T0, $n... TT> 
    static auto create( TT&&... args ) {

      $static_assert( sizeof( U0 ) <= sizeof( data ) );
      $static_assert( alignof( U0 ) <= 2 or alignof( void* ) == alignof( U0 ) );

      auto v = value< T0 >{};

      new( v.data ) U0{ args... };

      return v;

    }

    $t<$n... TT>
    auto operator()( TT... args ) {
      return (* (T0*) data)( args... );
    }

    T0* operator->() {
      
      return (T0*) data;

    }

    $t<$n U0> static char (& get_size_sfinae( int ) )[ U0::type_size ];
    $t<$n U0> static void* (& get_size_sfinae( ... ) )[2];


    alignas( alignof( void* ) ) unsigned char data[ sizeof( get_size_sfinae<T0>( 0 ) ) ];

  };


}

