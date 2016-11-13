#pragma once

#include <cstring>
#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/error.h"
#include "lib/buffer.h"
#include "lib/vector.h"
#include "lib/scope-guard.h"
#include "lib/handle.h"
#include "lib/alloc-default.h"

#include "error.h"

namespace lib {

  namespace os {


    struct file {

      using handle_t = handle< FILE* >;


      file( cstr path ) : _path{ path } { }


      vector_b load() {

        open();

        ssize_t file_size = size();

        vector_b data{ file_size, alloc_default::create( "file data" ) };

        while( data.size() < file_size ) {

          auto read = fread( data.data(), 1, file_size, _h );

          data.set_size( read );

          if( feof( _h ) or ferror( _h ) != 0 ) 

            $throw $error_file( _path, strerror( errno ) );
        }

        return move( data );
     }

     ssize_t size() {

        if( _size != -1 ) return _size;

        open();

        auto pos = tell();

        $on_return { seek( pos, SEEK_SET ); };

        seek( 0, SEEK_END );

        auto _size = tell();

        seek( 0, SEEK_SET );

        return _size;
      }

      bool exists() {

        FILE* f = fopen( _path, "rb" );

        $on_return { if( f ) fclose( f ); };

        return f ? true : false;
      }

      void close() { _h.~handle_t(); }

      void open() {

        if( _h ) return;

        FILE* f = fopen( _path, "r+b" );
        
        if( f == nullptr ) $throw $error_file( _path, strerror( errno ) );
        
        handle_t::deleter_t d = []( FILE* f ) { fclose( f ); };

        _h = handle_t{ f, d };
      }

      string get_line( string str = string{} ) {

        open();

        auto& buffer = global::get_buffer< char, 4096 >();

        auto ptr = fgets( buffer, $array_size( buffer ), _h );

        if( ptr == nullptr and ferror( _h ) )

          $throw $error_file( _path, strerror( errno ) );

        if( ptr ) {

          str.reserve( strlen( ptr ) + 1 );

          str << ptr;
        }

        return str;
      }

      ssize_t seek( ssize_t offset, int whence ) {

        auto r = fseek( _h, offset, whence );

        if( r != 0 )

          $throw $error_file( _path, strerror( errno ) );

        return r;
      }

      ssize_t tell() {

        auto offset = ftell( _h );

        if( offset == -1L )

          $throw $error_file( _path, strerror( errno ) );

        return offset;
      }

      bool eof() { return not (bool) _h or feof( _h ); }

      handle_t _h{};
      cstr _path{};
      ssize_t _size = -1;
    };
  


  }

}


