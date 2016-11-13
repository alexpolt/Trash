#pragma once


#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/strong-ptr.h"
#include "lib/url.h"
#include "lib/alloc-default.h"
#include "lib/alloc-empty.h"
#include "os/file.h"

#include "config.h"
#include "error.h"


namespace lib {

  namespace loader {


    struct loader_file {

      static strong_ptr< vector_b > load( url location ) {
      
        string path{ 64, alloc_default::create( "loader data path" ) };
        
        for( auto dir : config::file_dirs ) {

          path << dir << location.path();

          os::file f = os::file{ path.data() };

          if( f.exists() ) {

            auto data = f.load();

            auto name = data.get_allocator()->name();

            data.set_allocator( alloc_empty::create( name ) );

            return strong_ptr< vector_b >( new vector_b{ move( data ) }, name );
          }

          path.clear();
        }

        $throw $error_loader( location.source() );

        return strong_ptr< vector_b >{};
      }

    };



  }

}


