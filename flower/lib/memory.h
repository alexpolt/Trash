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

    TP<TN = void> stats_t& get_stats() { static stats_t stats; return stats; }


    auto alloc( ssize_t size, cstr file_line ) {

      get_stats().alloc.add( size, std::memory_order_relaxed );

      log::memory, file_line, " alloc(", size, "), stat = ", (ssize_t) get_stats().alloc, log::endl;

      auto ptr = ::malloc( size );

      if( ! ptr ) throw error_memory{ "malloc failed" };

      return ptr;

    };

    TP<TN T0>
    void free( out< T0* > ptr, ssize_t size, cstr file_line ) { 
      
      get_stats().alloc.sub( size, std::memory_order_relaxed );

      log::memory, file_line, " free(", size, "), stat = ", (ssize_t) get_stats().alloc, log::endl;

      ::free( *ptr ); 
      
      *ptr = nullptr; 
      
    };

  }


  #define $alloc( $0 ) lib::alloc::alloc( $0, __FILE__ ":" $str( __LINE__ ) )
  #define $free( $0, $1 ) lib::alloc::free( $0, $1, __FILE__ ":" $str( __LINE__ ) )


}

