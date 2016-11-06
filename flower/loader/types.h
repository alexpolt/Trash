#pragma once


#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/buffer.h"
#include "lib/strong-ptr.h"
#include "lib/url.h"
#include "lib/alloc-.h"
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

          if( f.exists() ) {

            auto& data = f.load();

            data.set_allocator( alloc_empty::create() );

            return lib::make_strong< vector_b >( move( data ) );
          }

          path.clear();
        }

        $throw $error_loader( location.data() );

        return strong_ptr< vector_b >{};
      }

    };



  }

}


