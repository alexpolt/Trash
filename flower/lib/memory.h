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


    struct stats_t {

      lib::atomic< ssize_t > alloc;

    };

    inline stats_t& get_stats() { static stats_t stats{}; return stats; }


    TP< ssize_t N0 >
    struct cache_t { 

      static const ssize_t size = N0;
      static const ssize_t size_max = 1024;

      ~cache_t() {
        
        for( auto i : range{ 0, size } ) {

          if( _ptr[ i ] ) {

            get_stats().alloc.sub( _size[ i ] );

            log::memory, "object ", _owner[ i ], ", free( ", _size[ i ], 

                        " ), stat = ", (ssize_t) get_stats().alloc, log::endl;

            ::free( _ptr[ i ] ); 

          }
        }
      }

      void* _ptr[ N0 ]; 
      void* _owner[ N0 ];
      ssize_t _size[ N0 ]; 
      ssize_t _index;
    };

    inline cache_t< 8 >& get_cache() { static cache_t< 8 > cache{}; return cache; }

 
    inline void* alloc( void* object, ssize_t size, cstr file_line ) {

      auto& stats = get_stats();

      stats.alloc.add( size );

      log::memory, "object ", object, " alloc( ", size, 

                   " ), stat = ", (ssize_t) get_stats().alloc, log::endl;

      auto& cache = get_cache();

      if( size <= cache.size_max )

        for( auto i : range{ 0, cache.size } ) {

          if( cache._ptr[ i ] and cache._size[ i ] == size )

            return move( cache._ptr[ i ] );
        }

      auto ptr = ::malloc( size );

      if( ! ptr ) throw error_memory{ "malloc failed" };

      return ptr;

    };


      TP<TN T0>
      inline void free( void* owner, out< T0* > ptr, ssize_t size, cstr file_line ) { 
      
      auto& stats = get_stats();


      auto& cache = get_cache();

      if( size > cache.size_max ) {

        stats.alloc.sub( size );

        log::memory, "object ", owner, ", free( ", size, " ), stat = ", (ssize_t) get_stats().alloc, log::endl;

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

        log::memory, "object ", cache._owner[ idx ], ", free( ", cache._size[ idx ], 

                     " ), stat = ", (ssize_t) get_stats().alloc, log::endl;

        ::free( cache._ptr[ idx ] ); 

        cache._ptr[ idx ] = ptr.get();
        cache._size[ idx ] = size;
        cache._owner[ idx ] = owner;

      }

      ptr = nullptr; 

      
    };


  }


  #define $alloc( $0, $1 ) lib::alloc::alloc( (void*)$0, $1, __FILE__ ":" $str( __LINE__ ) )
  #define $free( $0, $1, $2 ) lib::alloc::free( (void*)$0, $1, $2, __FILE__ ":" $str( __LINE__ ) )


}

