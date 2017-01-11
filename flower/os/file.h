#pragma once

#include <cstring>
#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/error.h"
#include "lib/buffer.h"
#include "lib/log.h"
#include "lib/to-string.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/scope-guard.h"
#include "lib/handle.h"
#include "lib/alloc-default.h"
#include "error.h"


namespace lib {

  namespace os {


    struct file : nocopy {

      using handle_t = handle< FILE* >;

      file() { }

      file( string path, cstr mode = "rb" ) : _path{ move( path ) }, _mode{ mode } { }

      auto create_error() { return $error_file( path(), strerror( errno ) ); }
      
      void load( vector_b& data ) {

        open();

        ssize_t file_size = size();

        log::os, $this, " loading ", file_size, " bytes", log::endl;

        data.reserve( file_size );

        ssize_t size_read{};

        while( data.size() < file_size ) {

          size_read += fread( data.data() + data.size(), 1, file_size, _h );

          data.set_size( data.size() + size_read );

          if( feof( _h ) or ferror( _h ) != 0 ) $throw create_error();
        }
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

      static bool exists( cstr path ) {

        FILE* f = fopen( path, "rb" );

        $on_return { if( f ) fclose( f ); };

        return f ? true : false;
      }

      void close() { _h.~handle_t(); }

      void open() {

        if( $this ) return;

        FILE* f = fopen( path(), _mode );
        
        if( f == nullptr ) $throw create_error();

        log::os, "opened file ", path(), log::endl;
        
        handle_t::deleter_t d = []( FILE* f ) { fclose( f ); };

        _h = handle_t{ f, d };
      }

      void get_line( string& str ) {

        open();

        auto& buffer = global::get_buffer< char, 4096 >();

        auto ptr = fgets( buffer, $array_size( buffer ), _h );

        if( ptr == nullptr and ferror( _h ) ) $throw create_error();

        if( ptr ) {

          str.reserve( strlen( ptr ) + 1 );

          str << ptr;
        }
      }

      TP<TN T>
      void read( vector< T >& data, ssize_t count ) {

        log::os, $this, " reading ", count, " elements of size ", $size( T ), log::endl;

        open();

        ssize_t size_read{};

        data.reserve( count );

        while( size_read < count ) {

          size_read += fread( data.data() + data.size(), $size( T ), count, _h );

          data.set_size( data.size() + size_read );

          if( feof( _h ) ) break;

          if( ferror( _h ) != 0 ) $throw create_error();
        }
      }

      ssize_t seek( ssize_t offset, int whence ) {

        auto r = fseek( _h, offset, whence );

        if( r != 0 ) $throw create_error();

        return r;
      }

      ssize_t tell() {

        auto offset = ftell( _h );

        if( offset == -1L ) $throw create_error();

        return offset;
      }

      bool eof() { return not (bool) _h or feof( _h ); }

      explicit operator bool() const { return (bool)_h; }

      cstr path() const { return _path.data(); }

      cstr to_string() const {

        if( $this )

          return lib::to_string( "file( %s, %p )", path(), (void*) _h.get() );

        else
          return lib::to_string( "file( %s, not opened )", path() );
      }

      handle_t _h{};
      string _path{};
      cstr _mode{};
      ssize_t _size = -1;
    };
  


  }

}


