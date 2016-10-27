#pragma once


#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/buffer.h"
#include "lib/shared-ptr.h"
#include "lib/url.h"
#include "os/file.h"

#include "config.h"
#include "error.h"


namespace lib {

  namespace loader {


    struct loader_file {

      static shared_ptr< vector_b > load( url location ) {
      
        auto path = lib::make_string( global::get_buffer< char >() );
        
        for( auto dir : config::file_dirs ) {

          path << dir << location.get_path();

          os::file f = os::file{ path.data() };

          if( f.exists() )

            return lib::make_shared< vector_b >( f.load() );

          path.clear();
        }

        $throw $error_loader( location.data() );

        return shared_ptr< vector_b >{ nullptr };
      }

    };



  }

}


