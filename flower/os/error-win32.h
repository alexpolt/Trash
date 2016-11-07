#pragma once

#include <cstring>
#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"


namespace lib {

  namespace os {


    struct error_win32 : error {

      error_win32( cstr file, int line, cstr func, cstr msg ) :

        error{ file, line, func, msg } {

        auto l = strlen( error::get_buffer() );

        auto ptr = error::get_buffer() + l;

        *ptr++ = ' ';

        FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, 
        
                        ptr, $array_size( error::get_buffer() ) - l - 1, nullptr );
      }

    };

    #define $error_win32( $0 ) error_win32{ __FILE__, __LINE__, __func__, $0 }


  }
}


