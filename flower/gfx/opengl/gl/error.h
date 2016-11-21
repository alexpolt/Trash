#pragma once

#include "os/error-win32.h"


namespace lib {

  namespace os {

  
    using error_api = error_win32;

    #define $error_api( $0 ) os::error_api{ $file_line, $0 }

  }
}


