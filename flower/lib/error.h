#pragma once

#include <cstdio>
#include "macros.h"
#include "types.h"

namespace lib {

  struct error {

    error( ) : _file_line{} { }

    error( cstr msg ) {

        snprintf( _file_line, $length( _file_line ), "%s", msg );
    }

    error( cstr file, int line, cstr func ) { 

        snprintf( _file_line, $length( _file_line ), "%s:%d::%s() -> ", file, line, func );

    }

    error( cstr file, int line, cstr func, cstr msg ) { 

        snprintf( _file_line, $length( _file_line ), "%s:%d::%s() -> %s", file, line, func, msg );

    }

    virtual ~error() {}

    virtual cstr what() const { return _file_line; }

    cstr to_string() const { return what(); }

    char _file_line[512];

  };

  #define $error( $0 ) lib::error{ __FILE__, __LINE__, __func__, $0 }


  struct error_object : error {

    error_object( cstr file, int line, cstr func, iid_t iid, cstr msg ) :
      error{ file, line, func } {

      snprintf( _buf, $length( _buf ), 
        "%s: object %d not found in object( %s )", error::what(), iid, msg );

    }

    cstr what() const override { return _buf; }

    char _buf[1024];

  };

  #define $error_object( $0, $1 ) lib::error_object{ __FILE__, __LINE__, __func__, $0, $1 }


  struct error_dispatch : error {

    error_dispatch( cstr file, int line, cstr func, cstr msg_a, cstr msg_b ) : 
      error{ file, line, func } {
        
      snprintf( _buf, $length( _buf ), 
        "%s: dispatch failed for object( %s ) and object( %s )", error::what(), msg_a, msg_b );

    }

    cstr what() const override { return _buf; }

    char _buf[1024];
 
  };

  #define $error_dispatch( $0, $1 ) lib::error_dispatch{ __FILE__, __LINE__, __func__, $0, $1 }


  struct error_input : error {

    using error::error;

  };

  #define $error_input( $0 ) lib::error_input{ __FILE__, __LINE__, __func__, $0 }


  struct error_not_implemented : error {

    using error::error;

  };

  #define $error_not_implemented( $0 ) lib::error_not_implemented{ __FILE__, __LINE__, __func__, $0 }


}




