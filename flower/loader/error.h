#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"


namespace lib {

  namespace loader {


    struct error_loader : error { 

      using error::error;
    };

    #define $error_loader( $0 ) error_loader{ __FILE__, __LINE__, __func__, $0 }

  }

}


