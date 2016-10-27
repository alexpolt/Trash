#pragma once

#include <cstring>
#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/error.h"
#include "lib/vector.h"
#include "lib/scope-guard.h"
#include "lib/handle.h"

#include "error.h"

namespace lib {

  namespace os {


    struct file {

      using handle_t = handle< FILE* >;


      file( cstr path ) : _path{ path } { }


      vector_b load() {

        open();

        ssize_t file_size = size();

        vector_b data{ file_size };

        while( data.size() < file_size ) {

          auto read = fread( data.data(), 1, file_size, _h );

          data.set_size( read );

          if( feof( _h ) or ferror( _h ) != 0 ) 

            $throw $error_file( _path, strerror( errno ) );
        }

        return move( data );
      }

     ssize_t size() {

        open();

        seek( 0, SEEK_END );

        auto size = tell();

        seek( 0, SEEK_SET );

        return size;
      }

      bool exists() {

        FILE* f = fopen( _path, "rb" );

        $on_return { if( f ) fclose( f ); };

        return f ? true : false;
      }

      void close() { _h.~handle_t(); }

      void open() {

        if( _h ) return;

        FILE* f = fopen( _path, "rb" );
        
        if( not f ) $throw $error_file( _path, strerror( errno ) );
        
        handle_t::deleter_t d = []( FILE* f ) { fclose( f ); };

        _h = handle_t{ f, d };
      }

      ssize_t seek( ssize_t offset, int whence ) {

        auto r = fseek( _h, offset, whence );

        if( r not_eq 0 )

          $throw $error_file( _path, strerror( errno ) );

        return r;
      }

      ssize_t tell() {

        auto offset = ftell( _h );

        if( offset == -1L )

          $throw $error_file( _path, strerror( errno ) );

        return offset;
      }

      handle_t _h;
      cstr _path;
    };
  


  }

}


