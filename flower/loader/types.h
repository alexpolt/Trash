#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/owner-ptr.h"


namespace lib {

  namespace loader {


    struct base : nocopy_vbase {

      virtual void load( vector_b&, ssize_t ) = 0;

      virtual void get_line( string& ) = 0;

      virtual vector_b load() = 0;

      virtual ssize_t size() = 0;
    };


    using loader_ptr = owner_ptr< base >;

  }
}


