#pragma once

#include "macros.h"
#include "types.h"
#include "error.h"
#include "assert.h"

namespace lib {


  $T<$N T0>
  struct buffer {

    buffer() { }

    buffer( T0* data, int size ) : _data{ data }, _size{ size } { 
      $assert( size > 0 );
    }


    T0* data() const { return _data; }

    int size() const { return _size; }

    void set_data( T0* data ) { _data = data; }

    void set_size( int size ) { 
      $assert( size > 0 );
      _size = size; 
    }


    T0* _data;
    int _size;

  };


}


