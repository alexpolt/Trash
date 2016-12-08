#pragma once

#include <cstdlib>
#include "macros.h"
#include "assert.h"
#include "error.h"
#include "types.h"
#include "log.h"
#include "atomic.h"


namespace lib {


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
        
        log::malloc, "freeing memory cache", log::endl;

        for( auto i : range{ 0, size } ) {

          if( _ptr[ i ] ) {

            get_stats().alloc.sub( _size[ i ] );

            log::malloc, "free from cache( ", _size[ i ], ", ", (void*) _ptr[ i ];
            log::malloc, " ); total = ", get_stats().alloc.load(), log::endl;

            ::free( _ptr[ i ] ); 

          }
        }

        destroyed = true;
      }

      volatile bool destroyed = false;

      void* _ptr[ size ]; 
      ssize_t _size[ size ]; 
      ssize_t _index;
    };


    inline cache_t& get_cache() { static cache_t cache{}; return cache; }

 
    inline void* alloc( ssize_t size ) {

      auto& stats = get_stats();
      auto& cache = get_cache();

      if( not cache.destroyed and cache.enabled and size <= cache.size_max )

        for( auto i : range{ 0, cache.size } ) {

          if( cache._ptr[ i ] and cache._size[ i ] == size ) {

            log::malloc, "alloc from cache( ", size, " ) = ", (void*) cache._ptr[ i ], log::endl;

            return move( cache._ptr[ i ] );
          }
        }

      auto ptr = ::malloc( size );

      if( ! ptr ) $throw error_memory{ $file_line, "malloc failed" };
      
      stats.alloc.add( size );

      log::malloc, "malloc( ", size, " ) = ", (void*)ptr, ", total = ", stats.alloc.load(), log::endl;

      return ptr; 
    }


    inline void free( void* ptr, ssize_t size  ) { 
    
      auto& stats = get_stats();
      auto& cache = get_cache();

      if( not cache.enabled or cache.destroyed or size > cache.size_max ) {

        stats.alloc.sub( size );

        log::malloc, "free( ", size, ", ", ptr, " ), total = ", stats.alloc.load(), log::endl;

        ::free( ptr ); 

      } else {

        for( auto i : range{ 0, cache.size } ) {

          if( ! cache._ptr[ i ] ) {

            log::malloc, "free to cache( ", size, ", ", ptr, " ), total = ", stats.alloc.load(), log::endl;

            cache._ptr[ i ] = ptr;
            cache._size[ i ] = size;

            return;
          }
        }

        auto idx = cache._index++ % cache.size;

        stats.alloc.sub( cache._size[ idx ] );

        log::malloc, "free from cache( ", cache._size[ idx ], ", "; 
        log::malloc, cache._ptr[ idx ], " ), total = ", stats.alloc.load(), log::endl;

        ::free( cache._ptr[ idx ] ); 

        cache._ptr[ idx ] = ptr;
        cache._size[ idx ] = size;
      }

    }


}


