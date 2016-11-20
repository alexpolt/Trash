#pragma once

#include <cstdio>

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"
#include "lib/log.h"


namespace lib {

  namespace os {


    struct error_win32 : error {

      error_win32( cstr file, cstr msg ) {

        auto ptr = error::get_buffer();

        ptr += snprintf( ptr, $array_size( error::get_buffer() ), "%s: ", file );

        FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, 
        
                        ptr, $array_size( error::get_buffer() ), nullptr );

        log::error, error::get_buffer(), log::endl;
      }

    };

    #define $error_win32( $0 ) os::error_win32{ $file_line, $0 }


  }
}


