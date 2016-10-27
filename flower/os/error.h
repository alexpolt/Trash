#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"


namespace lib {

  namespace os {


    struct error_file : error { 

      error_file( cstr file, int line, cstr func, cstr path, cstr strerror  ) : 

      error{ file, line, func } {

        auto l = strlen( error::get_buffer() );

        auto ptr = error::get_buffer() + l;

        snprintf( ptr, $length( error::get_buffer() ) - l, "\"%s\" %s", path, strerror );
      }
    };

    #define $error_file( $0, $1 ) error_file{ __FILE__, __LINE__, __func__, $0, $1 }

  }

}


