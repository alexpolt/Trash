#pragma once

#include "macros.h"
#include "types.h"
#include "assert.h"
#include "atomic.h"
#include "vector.h"
#include "to-string.h"
#include "log.h"

namespace lib {


  struct locker_t : nocopy {

    using deleter_t = void (*)( void* );

    locker_t() {

      _lock_map.reserve( 4 );
    }

    ~locker_t() {

      $assert( _lock_map.size() == 0, "lock_map size is not zero");
    }

    void lock( void* ptr, deleter_t deleter ) {

      log::lock, "lock( ", ptr;

      auto it = find( _lock_map, ptr );

      int counter = 1;

      if( it )

        counter = it->counter.add( 1 ) + 1;

      else

        _lock_map.push_back( { ptr, { counter }, deleter } );

      log::lock, ", ", counter, " )", log::endl;
    }

    void lock( void* ptr ) {

      log::lock, "lock( ", ptr;

      auto it = find( _lock_map, ptr );

      $assert( it, "lock not found" ); 

      auto counter = it->counter.add( 1 ) + 1;

      log::lock, ", ", counter, " )", log::endl;
    }

    bool unlock( void* ptr, bool is_weak = false ) {

      auto it = find( _lock_map, ptr );

      $assert( it, "lock not found" );

      auto counter = it->counter.sub( 1 );

      log::lock, "unlock( ", ptr, ", is_weak = ", is_weak, ", ", counter, " )", log::endl;

      if( counter == 1 ) {
        
        if( not is_weak ) it->deleter( ptr );

        _lock_map.erase( it );

        return true;
      }

      return false;
    }

    int use_count( void* ptr ) { 

      auto it = find( _lock_map, ptr );

      if( not it ) return 0;

      return it->counter.load();
    }

    struct lock_node {

      bool operator==( void* ptr2 ) const { return ptr == ptr2; }

      bool operator<( void* ptr2 ) const { return less( ptr, ptr2 ); }

      cstr to_string() const { return lib::to_string( "lock( %p, %d )", ptr, counter.load() ); }

      void* ptr;
      atomic< int > counter;
      deleter_t deleter;
    };


    vector< lock_node > _lock_map;
  };


  namespace global {

    TP<TN T0>
    locker_t locker{};
  }


}



