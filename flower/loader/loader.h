#pragma once

#include <cstring>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/value.h"

#include "types.h"


namespace lib {

  namespace loader {


    enum class scheme { file, web, db, null };


    scheme get_scheme( cstr location );


    value< loader > get_loader( cstr location ) {

      scheme s = get_scheme( location );

      if( s == schema::file ) {
          
        return value< loader >::create< loader_file >();
      }


      return value< loader >{};
    }

    
    scheme get_scheme( cstr location ) {

      if( strstr( "file://", location ) == location ) return scheme::file;

      if( strstr( "http://", location ) == location ) return scheme::web;

      if( strstr( "db://", location ) == location ) return scheme::db;

      $assert( "only file, http or db scheme is supported" );

      return scheme::null;
    }
    

  }

}


