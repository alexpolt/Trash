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

      snprintf( _buffer, $array_size( _buffer ), "%s", msg );
    }

    error( cstr file, cstr msg ) : error() { 

      snprintf( _buffer, $array_size( _buffer ), "%s: %s", file, msg );
    }

    virtual ~error() {}

    virtual cstr what() const { return _buffer; }

    cstr to_string() const { return what(); }

    buffer_t get_buffer() { return _buffer; }

    buffer_t _buffer;
  };

  #define $error( $0 ) lib::error{ $file_line, $0 }


  struct error_input : error {

    using error::error;

  };

  #define $error_input( $0 ) lib::error_input{ $file_line, $0 }


  struct error_not_implemented : error {

    using error::error;

  };

  #define $error_not_implemented() lib::error_not_implemented{ $file_line, "not implemented" }


}




