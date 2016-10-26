#pragma once


#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/buffer.h"
#include "lib/shared-ptr.h"
#include "os/file.h"

#include "config.h"


namespace lib {

  namespace loader {

    using vector = vector< byte >;

    struct loader {

      virtual shared_ptr< vector > load() const = 0;
    };

    struct loader_file : loader {

      shared_ptr< vector > load( cstr location ) const override {
      
        auto path = lib::make_string( global::get_buffer() );
        
        path << config::file_path << location;

        os::file f{ path.data() };

        return lib::make_shared< vector >( move( f.data() ) );
      }

    };

  }

}


