#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/vector.h"
#include "lib/string.h"
#include "lib/alloc-default.h"
#include "lib/url.h"
#include "render/types.h"
#include "render/buffer.h"
#include "loader.h"
#include "cache.h"


namespace lib {

  namespace loader {

  namespace buffer {


    inline vbuffer_ptr load( url location, bool cache = false ) {

      if( cache ) {

        auto it = cache<>[ location ];

        if( it ) 
          
          return static_cast< vbuffer_ptr > ( *it );
      }

      auto loader = loader::create( url );

      string str{ 32, alloc_default::create( "buffer loader string" ) };

      vbuffer::vector_type v{ 256, alloc_default::create( url.path() ) };

      ssize_t size = 0;

      float f0, f1, f2;

      while( true ) {

        loader->get_line( str );

        if( not str ) break;

        sscanf( str.data(), "%f%f%f", &f0, &f1, &f2 );

        v << f0 << f1 << f2;
      }

      auto ptr = new render::vbuffer{ resource::format::float3, move( v ) };

      auto buffer = strong_ptr< render::vbuffer >{ ptr, url.path() };

      if( cache ) cache<>.insert( location, buffer.lock() );

      return move( buffer );
    }


  }
  }
}


