#pragma once

#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"



namespace lib {

  namespace os {


    struct file {

      using vector = lib::vector< byte >;

      file( cstr path ) : _path{ path } { }
      
      vector load() const {

        handle< FILE > file0 = fopen( _path, "rb" );



        while( feof( file0 ) ) {
          _data << 
        }
      }


      cstr _path;
    };
  

  }

}


