#pragma once

#include <cstdlib>
#include "macros.h"
#include "assert.h"
#include "error.h"
#include "types.h"
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

            log::memory, "object ", _owner[ i ], ", free( ", _size[ i ], ", ", (void*) _ptr[ i ];
            log::memory, " ); total = ", get_stats().alloc.load(), log::endl;

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

      auto& cache = get_cache();
      
      $assert( not cache.destroyed, "alloc failed, memory cache was destroyed" );

      if( cache.enabled and size <= cache.size_max )

        for( auto i : range{ 0, cache.size } ) {

          if( cache._ptr[ i ] and cache._size[ i ] == size ) {

            log::memory, "object ", object, " cache alloc( ", size, " ) = ";
            log::memory, (void*)cache._ptr[ i ], log::endl;

            return move( cache._ptr[ i ] );
          }
        }

      auto ptr = ::malloc( size );

      if( ! ptr ) $throw error_memory{ "malloc failed" };
      
      stats.alloc.add( size );

      log::memory, "object ", object, " malloc( ", size, " ) = ", (void*)ptr, "; ";
      log::memory, "total = ", stats.alloc.load(), log::endl;


      return ptr; 
    }


    inline void free( void* owner, void* ptr, ssize_t size, cstr file_line ) { 
    
      auto& stats = get_stats();
      auto& cache = get_cache();

      if( not cache.enabled or cache.destroyed or size > cache.size_max ) {

        stats.alloc.sub( size );

        log::memory, "object ", owner, ", free( ", size, ", ", ptr, " ); ";
        log::memory, "total = ", stats.alloc.load(), log::endl;

        ::free( ptr ); 

      } else {

        for( auto i : range{ 0, cache.size } ) {

          if( ! cache._ptr[ i ] ) {

            log::memory, "object ", owner, ", free to cache( ", size, ", "; 
            log::memory, ptr, " ); total = ", stats.alloc.load(), log::endl;

            cache._ptr[ i ] = ptr;
            cache._size[ i ] = size;
            cache._owner[ i ] = owner;

            return;
          }
        }

        auto idx = cache._index++ % cache.size;

        stats.alloc.sub( cache._size[ idx ] );

        log::memory, "object ", cache._owner[ idx ], ", free from cache( ", cache._size[ idx ], ", "; 
        log::memory, cache._ptr[ idx ], " ); total = ", stats.alloc.load(), log::endl;

        ::free( cache._ptr[ idx ] ); 

        cache._ptr[ idx ] = ptr;
        cache._size[ idx ] = size;
        cache._owner[ idx ] = owner;

      }
    }


  }


  #define $alloc( $0, $1 ) lib::memory::alloc( (void*)$0, $1, __FILE__ ":" $str( __LINE__ ) )
  
  #define $free( $0, $1, $2 ) lib::memory::free( (void*)$0, $1, $2, __FILE__ ":" $str( __LINE__ ) )


}

