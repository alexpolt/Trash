#pragma once

#include <cstdlib>
#include "macros.h"
#include "error.h"
#include "types.h"
#include "ptr.h"
#include "log.h"
#include "atomic.h"

namespace lib {

  namespace alloc {

    struct error_memory : error { using error::error; };

    TP< ssize_t N0 >
    struct cache_t { 
      void* ptr[ N0 ]; 
      void* owner[ N0 ];
      ssize_t size[ N0 ]; 
      ssize_t index;
    };

    struct stats_t {

      lib::atomic< ssize_t > alloc;
      
      cache_t< 8 > cache;

    };

    inline stats_t& get_stats() { static stats_t stats{}; return stats; }
/*
    TP<TN T0>
    inline void free( void* object, out< T0* > ptr, ssize_t size, cstr file_line );

    inline void* alloc( void* object, ssize_t size, cstr file_line ) {

      auto& stats = get_stats();

      stats.alloc.add( size );

      log::memory, object, " alloc(", size, "), stat = ", (ssize_t) get_stats().alloc, log::endl;

      for( auto& e : stats.cache ) {

        if( e.data and e.size == size )

          return move( e.data );
      }

      auto ptr = ::malloc( size );

      if( ! ptr ) throw error_memory{ "malloc failed" };

      return ptr;

    };

    TP<TN T0>
    inline void free( void* owner, out< T0* > ptr, ssize_t size, cstr file_line ) { 
      
      auto& stats = get_stats();

      stats.alloc.sub( size );

      for( auto& e : stats.cache ) {

        if( e.data == nullptr ) {

          e.data = *ptr;
          e.size = size;
          return;
        }
      }

      auto index = stats.cache_index++;

      auto& block = stats.cache[ index % $length( stats.cache ) ];

      auto block_old = block;

      block.data = *ptr;
      block.size = size;

      *ptr = (T0*) block_old.data;
      size = block_old.size;

      log::memory, owner, " free(", size, "), stat = ", (ssize_t) get_stats().alloc, log::endl;

      ::free( *ptr ); 
      
      *ptr = nullptr; 

      
    };

  }

*/
  #define $alloc( $0, $1 ) lib::alloc::alloc( (void*)$0, $1, __FILE__ ":" $str( __LINE__ ) )
  #define $free( $0, $1, $2 ) lib::alloc::free( (void*)$0, $1, $2, __FILE__ ":" $str( __LINE__ ) )


}

