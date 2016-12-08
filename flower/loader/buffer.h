#pragma once

#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/log.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/alloc-default.h"
#include "lib/url.h"
#include "render/buffer.h"
#include "loader.h"
#include "cache.h"


namespace lib {

  namespace loader {

  namespace buffer {


    inline render::vbuffer_ptr load( url location, bool cache = false ) {

      log::loader, "load buffer ", location.path(), log::endl;

      if( cache ) {

        auto it = global::cache<>[ location ];

        if( it ) {

          log::loader, "get from cache ", *it, log::endl;
          
          return static_cast< render::vbuffer_ptr > ( *it );
        }
      }

      auto loader = loader::create( location );

      string str{ 32, alloc_default::create( "buffer loader get_line" ) };

      render::vbuffer::vector_type data{ 256, alloc_default::create( location.path() ) };

      float f0, f1, f2;

      while( true ) {

        loader->get_line( str );

        if( not str ) break;

        auto result = sscanf( str.data(), "%f%f%f", &f0, &f1, &f2 );

        if( not result or result == EOF ) break;

        str.clear();

        data << f0 << f1 << f2;
      }

      log::loader, "loaded ", data.size_bytes(), " bytes", log::endl;

      auto ptr = new render::vbuffer{ render::res::format::float3, move( data ) };

      auto buffer = strong_ptr< render::vbuffer >{ ptr, location.path() };

      if( cache ) {

        log::loader, "store in cache ", *buffer, log::endl;

        global::cache<>.insert( location, buffer.lock() );
      }

      return move( buffer );
    }


  }
  }
}


