#pragma once

#include <cstring>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/url.h"
#include "lib/vector.h"
#include "lib/strong-ptr.h"

#include "types.h"
#include "error.h"


namespace lib {

  namespace loader {


    strong_ptr< vector_b > load( url location ) {

      if( location.get_scheme() == url::scheme::file ) {

        return loader_file::load( location );
      }
      
      $assert( false, "no such scheme" );

      return shared_ptr< vector_b >{};
    }


    strong_ptr< vector_b > load( cstr path ) {

      auto location = lib::url::create( path );

      return load( location );
    }

   

  }

}


