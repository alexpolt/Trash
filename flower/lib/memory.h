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

    struct block_t { ssize_t size; void* data; };

    struct stats_t {

      lib::atomic< ssize_t > alloc;

      block_t cache;

    };

    inline stats_t& get_stats() { static stats_t stats{}; return stats; }

    TP<TN T0>
    inline void free( void* object, out< T0* > ptr, ssize_t size, cstr file_line );

    inline void* alloc( void* object, ssize_t size, cstr file_line ) {

      auto& stats = get_stats();

      stats.alloc.add( size );

      log::memory, object, " alloc(", size, "), stat = ", (ssize_t) get_stats().alloc, log::endl;

      if( stats.cache.size == size and stats.cache.data ) 

          return move( stats.cache.data );

      if( stats.cache.data )

        free( nullptr, $out( stats.cache.data ), stats.cache.size, __FILE__ );

      auto ptr = ::malloc( size );

      if( ! ptr ) throw error_memory{ "malloc failed" };

      return ptr;

    };

    TP<TN T0>
    inline void free( void* object, out< T0* > ptr, ssize_t size, cstr file_line ) { 
      
      auto& stats = get_stats();

      stats.alloc.sub( size );

      if( stats.cache.data == nullptr ) {
        stats.cache.data = *ptr;
        stats.cache.size = size;
        return;
      }

      log::memory, object, " free(", size, "), stat = ", (ssize_t) get_stats().alloc, log::endl;

      ::free( *ptr ); 
      
      *ptr = nullptr; 
      
    };

  }


  #define $alloc( $0, $1 ) lib::alloc::alloc( (void*)$0, $1, __FILE__ ":" $str( __LINE__ ) )
  #define $free( $0, $1, $2 ) lib::alloc::free( (void*)$0, $1, $2, __FILE__ ":" $str( __LINE__ ) )


}

