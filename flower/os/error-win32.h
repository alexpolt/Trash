#pragma once

#include <cstring>
#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"


namespace lib {

  namespace os {


    struct error_window : error {

      error_window( cstr file, int line, cstr func, cstr msg ) :

        error{ file, line, func, msg } {

        auto l = strlen( error::get_buffer() );

        auto ptr = error::get_buffer() + l;

        *ptr++ = ' ';

        FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, 
        
                        ptr, $array_size( error::get_buffer() ) - l - 1, nullptr );
      }

    };

    #define $error_window( $0 ) error_window{ __FILE__, __LINE__, __func__, $0 }


  }
}


