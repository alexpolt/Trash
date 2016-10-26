#pragma once

#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"



namespace lib {

  namespace os {


    struct file {

      file( cstr path ) : _path{ path } { }

      cstr _path;
    };
  

  }

}


