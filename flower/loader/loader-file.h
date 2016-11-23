#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/url.h"
#include "lib/alloc-default.h"
#include "lib/alloc-empty.h"
#include "os/file.h"
#include "types.h"
#include "config.h"
#include "error.h"


namespace lib {

  namespace loader {


    struct loader_file : loader {


      loader_file( url location ) : _location{ location } { }


      void open() {

        if( _file ) return;

        string path{ 256, alloc_default::create( "file loader path" ) };

        for( auto dir : config::file_dirs ) {

          path << dir << _location.location();

          os::file f = os::file{ path.data() };

          if( f.exists() ) { 

            _file = move( f );

            return;
          }

          path.clear();
        }

        $throw $error_loader( _location.path() );
      }

      void get_line( string& str ) override { 

        open();

        _file.get_line( str );
      }

      vector_b load() override {
      
        open();
        
        auto data = _file.load();

        return vector_b{ move( data ) };
      }

      void load( vector_b& data, ssize_t size  ) override {

        $error_loader( "not implemented" );
      }

      ssize_t size() override {

        return _file.size();
      }

      url _location;
      os::file _file;
    };



  }

}


