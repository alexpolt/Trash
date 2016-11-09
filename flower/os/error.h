#pragma once

#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"


namespace lib {

  namespace os {


    struct error_file : error { 

      error_file( cstr file, cstr path, cstr strerror  ) {

        auto ptr = error::get_buffer();

        snprintf( ptr, $array_size( error::get_buffer() ), "%s: \"%s\" %s", file, path, strerror );
      }
    };


    #define $error_file( $0, $1 ) error_file{ $file_line, $0, $1 }



  }
}


