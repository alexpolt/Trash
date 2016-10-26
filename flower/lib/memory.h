#pragma once

#include <cstdlib>
#include "macros.h"
#include "assert.h"
#include "error.h"
#include "types.h"
#include "out-ref.h"
#include "log.h"
#include "atomic.h"

namespace lib {

  namespace memory {

    struct error_memory : error { using error::error; };

    struct stats_t {

      lib::atomic< ssize_t > alloc;

    };

    inline stats_t& get_stats() { static stats_t stats{}; return stats; }


    struct cache_t { 

      static constexpr bool enabled = true;
      static constexpr ssize_t cache_size = 16;
      static constexpr ssize_t size = cache_size;
      static constexpr ssize_t size_max = 512;

      ~cache_t() {
        
        for( auto i : range{ 0, size } ) {

          if( _ptr[ i ] ) {

            get_stats().alloc.sub( _size[ i ] );

            log::memory, "object ", _owner[ i ], ", free( ", _size[ i ], 

                        " ), memsize = ", (ssize_t) get_stats().alloc, log::endl;

            ::free( _ptr[ i ] ); 

          }
        }

        destroyed = true;
      }

      volatile bool destroyed = false;

      void* _ptr[ size ]; 
      void* _owner[ size ];
      ssize_t _size[ size ]; 
      ssize_t _index;
    };


    inline cache_t& get_cache() { static cache_t cache{}; return cache; }

 
    inline void* alloc( void* object, ssize_t size, cstr file_line ) {

      auto& stats = get_stats();

      log::memory, "object ", object, " alloc( ", size, " ) ", log::endl;

      auto& cache = get_cache();
      
      $assert( not cache.destroyed, "alloc failed, memory cache was destroyed" );

      if( cache.enabled and size <= cache.size_max )

        for( auto i : range{ 0, cache.size } ) {

          if( cache._ptr[ i ] and cache._size[ i ] == size ) 

            return move( cache._ptr[ i ] );
        }

      auto ptr = ::malloc( size );

      stats.alloc.add( size );

      log::memory, "memsize = ", (ssize_t) stats.alloc, log::endl;

      if( ! ptr ) $throw error_memory{ "malloc failed" };

      return ptr; 
    }


    TP<TN T0>
    inline void free( void* owner, out_ref< T0* > ptr, ssize_t size, cstr file_line ) { 
    
      auto& stats = get_stats();
      auto& cache = get_cache();

      if( not cache.enabled or cache.destroyed or size > cache.size_max ) {

        stats.alloc.sub( size );

        log::memory, "object ", owner, ", free( ", size, " ), memsize = ", (ssize_t) stats.alloc, log::endl;

        ::free( ptr.get() ); 

      } else {

        for( auto i : range{ 0, cache.size } ) {

          if( ! cache._ptr[ i ] ) {

            cache._ptr[ i ] = ptr.get();
            cache._size[ i ] = size;
            cache._owner[ i ] = owner;
            ptr = nullptr; 

            return;
          }
        }

        auto idx = cache._index++ % cache.size;

        stats.alloc.sub( cache._size[ idx ] );

        log::memory, "object ", cache._owner[ idx ], ", free cache( ", cache._size[ idx ], 

                     " ), memsize = ", (ssize_t) stats.alloc, log::endl;

        ::free( cache._ptr[ idx ] ); 

        cache._ptr[ idx ] = ptr.get();
        cache._size[ idx ] = size;
        cache._owner[ idx ] = owner;

      }

      ptr = nullptr; 
    }


  }


  #define $alloc( $0, $1 ) lib::memory::alloc( (void*)$0, $1, __FILE__ ":" $str( __LINE__ ) )
  
  #define $free( $0, $1, $2 ) lib::memory::free( (void*)$0, $1, $2, __FILE__ ":" $str( __LINE__ ) )


}

