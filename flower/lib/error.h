#pragma once


namespace lib {

  struct error {

    error( cstr file, uint line, cstr func ) { 

        snprintf( file_line, sizeof( file_line ), "%s:%d::%s() -> ", file, line, func );

    }

    error( cstr file, uint line, cstr func, cstr msg ) { 

        snprintf( file_line, sizeof( file_line ), "%s:%d::%s() -> %s", file, line, func, msg );

    }

    virtual ~error() {}

    virtual char const* what() const {
      return file_line;
    }

    char file_line[512];

  };

  #define $error( $0 ) lib::error{ __FILE__, __LINE__, __func__, $0 }


  struct error_object : error {

    error_object( cstr file, uint line, cstr func, iid_t iid, cstr msg ) :
      error{ file, line, func } {

      snprintf( buf, sizeof( buf ), 
        "%s: object %d not found in object( %s )", error::what(), iid, msg );

    }

    cstr what() const override {

      return buf;

    }

    char buf[1024];

  };

  #define $error_object( $0, $1 ) lib::error_object{ __FILE__, __LINE__, __func__, $0, $1 }


  struct error_dispatch : error {

    error_dispatch( cstr file, uint line, cstr func, cstr msg_a, cstr msg_b ) : 
      error{ file, line, func } {
        
      snprintf( buf, sizeof( buf ), 
        "%s: dispatch failed for object( %s ) and object( %s )", error::what(), msg_a, msg_b );

    }

    cstr what() const override {

      return buf;

    }

    char buf[1024];
 
  };

  #define $error_dispatch( $0, $1 ) lib::error_dispatch{ __FILE__, __LINE__, __func__, $0, $1 }


  struct error_input : error {

    using error::error;

  };

  #define $error_input( $0 ) lib::error_input{ __FILE__, __LINE__, __func__, $0 }



}




