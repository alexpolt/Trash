#pragma once

#include <cstring>
#include <cstdio>
#include "macros.h"
#include "types.h"
#include "buffer.h"

namespace lib {

  struct error {

    static constexpr ssize_t buffer_size = 256;

    error( ) : _buffer{ global::get_buffer< error, buffer_size >() } { }

    error( cstr msg ) : error() {

      snprintf( _buffer, $length( _buffer ), "%s", msg );
    }

    error( cstr file, int line, cstr func ) : error() { 

      snprintf( _buffer, $length( _buffer ), "%s:%d::%s() -> ", file, line, func );
    }

    error( cstr file, int line, cstr func, cstr msg ) : error() { 

      snprintf( _buffer, $length( _buffer ), "%s:%d::%s() -> %s", file, line, func, msg );
    }

    virtual ~error() {}

    virtual cstr what() const { return _buffer; }

    cstr to_string() const { return what(); }

    char ( &_buffer )[ buffer_size ]; 
  };

  #define $error( $0 ) lib::error{ __FILE__, __LINE__, __func__, $0 }


  struct error_object : error {

    error_object( cstr file, int line, cstr func, iid_t iid, cstr msg ) : error{ file, line, func } {

      auto l = strlen( error::_buffer );

      auto ptr = error::_buffer + l;

      snprintf( ptr, $length( error::_buffer ) - l, ": object %d not found in object( %s )", iid, msg );
    } 
  };

  #define $error_object( $0, $1 ) lib::error_object{ __FILE__, __LINE__, __func__, $0, $1 }


  struct error_dispatch : error {

    error_dispatch( cstr file, int line, cstr func, cstr msg_a, cstr msg_b ) : error{ file, line, func } {
        
      auto l = strlen( error::_buffer );

      auto ptr = error::_buffer + l;

      snprintf( ptr, $length( error::_buffer ) - l, ": dispatch failed for ( %s ) and ( %s )", msg_a, msg_b );
    }
  };

  #define $error_dispatch( $0, $1 ) lib::error_dispatch{ __FILE__, __LINE__, __func__, $0, $1 }


  struct error_input : error {

    using error::error;

  };

  #define $error_input( $0 ) lib::error_input{ __FILE__, __LINE__, __func__, $0 }


  struct error_not_implemented : error {

    using error::error;

  };

  #define $error_not_implemented() lib::error_not_implemented{ __FILE__, __LINE__, __func__, "not implemented" }


}




