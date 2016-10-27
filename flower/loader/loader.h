#pragma once

#include <cstring>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/value.h"
#include "lib/url.h"

#include "types.h"
#include "error.h"

namespace lib {

  namespace loader {


    value< loader > get_loader( url );
    

    shared_ptr< vector_b > load( url location ) {

      auto loader = get_loader( location );

      return loader->load( location );
    }
    
    shared_ptr< vector_b > load( cstr path ) {

      auto location = lib::url::create( path );

      return load( location );
    }

    
    value< loader > get_loader( url location ) {

      if( location.get_scheme() == url::scheme::file ) {
          
        return value< loader >::create< loader_file >();
      }

      return value< loader >{};
    }   


   

  }

}


