#pragma once

#include "common.h"

namespace global {

  $T<$N T0, int N0>
  struct buffer_t : lib::nocopy {

    buffer_t() { }

    buffer_t( cstr str )  { 

      if( strlen( str ) >= N0 ) 
        throw $error_input( "string length exceeds global::buffer size" );

      strcpy( _data, str );
    }

    T0* data() { return _data; }
    
    int size() { return N0; }

    thread_local static T0 _data[ N0 ];
  };

  $T<$N T0, int N0>
  thread_local T0 buffer_t< T0, N0 >::_data[ N0 ]; 

  using buffer = buffer_t< char, 256 >;

}

