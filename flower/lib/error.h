#pragma once


namespace lib {

  struct error {

    error( cchar_ptr file, uint line ) { 

        snprintf( file_line, sizeof( file_line ), "%s:%d", file, line );

    }

    error( cchar_ptr file, uint line, cchar_ptr msg ) { 

        snprintf( file_line, sizeof( file_line ), "%s:%d %s", file, line, msg );

    }

    virtual ~error() {}

    virtual char const* what() const {
      return file_line;
    }

    char file_line[512];

  };

  #define $error( $0 ) error{ __FILE__, __LINE__,$0 }


  struct error_object : error {

    error_object( cchar_ptr file, uint line, oid_t oid, cchar_ptr msg ) :
      error{ file, line } {

      snprintf( buf, sizeof( buf ), 
        "%s: object %d not found in object( %s )", error::file_line, oid, msg );

    }

    cchar_ptr what() const override {

      return buf;

    }

    char buf[1024];

  };

  #define $error_object( $0, $1 ) error_object{ __FILE__, __LINE__, $0, $1 }


  struct error_dispatch : error {

    error_dispatch( cchar_ptr file, uint line, cchar_ptr msg_a, cchar_ptr msg_b ) : 
      error{ file, line } {
        
      snprintf( buf, sizeof( buf ), 
        "%s: dispatch failed for object( %s ) and object( %s )", error::file_line, msg_a, msg_b );

    }

    cchar_ptr what() const override {

      return buf;

    }

    char buf[1024];
 
  };

  #define $error_dispatch( $0, $1 ) error_dispatch{ __FILE__, __LINE__, $0, $1 }


  struct error_input : error {

    using error::error;

  };

  #define $error_input() error_input{ __FILE__, __LINE__, "bad input" }



}




