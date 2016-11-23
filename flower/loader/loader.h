#pragma once

#include <cstring>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/value.h"
#include "lib/url.h"
#include "types.h"
#include "error.h"
#include "loader-file.h"


namespace lib {

  namespace loader {


    inline value< loader > create( url location ) {

      if( location.scheme() == url::scheme::file ) {

        return valule< loader >::create< loader_file >( location );
      }
      
      $assert( false, "no such scheme" );

      return value< loader >{};
    }


    inline auto create( cstr path ) {

      auto location = lib::url::create( path );

      return load( location );
    }

   

  }

}


