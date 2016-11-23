#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/vector.h"
#include "lib/string.h"


namespace lib {

  namespace loader {


    struct loader {

      virtual ~loader() { }

      virtual void load( vector_b&, ssize_t ) = 0;

      virtual get_line( string& ) = 0;

      virtual vector_b load() = 0;

      virtual ssize_t size() = 0;
    };

  }
}


