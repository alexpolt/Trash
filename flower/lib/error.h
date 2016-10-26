#pragma once

#include <cstring>
#include <cstdio>
#include "macros.h"
#include "types.h"
#include "buffer.h"

namespace lib {

  struct error {

    static constexpr ssize_t buffer_size = 256;

    using buffer_t = char (&)[ buffer_size ];


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

    buffer_t get_buffer() { return _buffer; }

    buffer_t _buffer;
  };

  #define $error( $0 ) lib::error{ __FILE__, __LINE__, __func__, $0 }


  struct error_input : error {

    using error::error;

  };

  #define $error_input( $0 ) lib::error_input{ __FILE__, __LINE__, __func__, $0 }


  struct error_not_implemented : error {

    using error::error;

  };

  #define $error_not_implemented() lib::error_not_implemented{ __FILE__, __LINE__, __func__, "not implemented" }


}




