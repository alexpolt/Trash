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

      _lock_map.reserve( 8 );
    }

    ~locker_t() {

      $assert( _lock_map.size() == 0, "lock_map size is not zero");
    }

    void lock( void* ptr, deleter_t deleter ) {

      log::lock, "lock( ", ptr, " ), size1 = ", _lock_map.size();

      auto it = find( _lock_map, ptr );

      if( it )
        it->counter.add( 1 );
      else
        _lock_map.push_back( { ptr, {1}, deleter } );

      log::lock, ", size2 = ", _lock_map.size(), log::endl;
    }

    void lock( void* ptr ) {

      log::lock, "lock( ", ptr, " ), size1 = ", _lock_map.size();

      auto it = find( _lock_map, ptr );

      $assert( it, "lock not found" ); 

      it->counter.add( 1 );

      log::lock, ", size2 = ", _lock_map.size(), log::endl;
    }

    void unlock( void* ptr, bool is_weak = false ) {

      log::lock, "unlock( ", ptr, ", destruct = ", destruct, " ), size1 = ", _lock_map.size();

      auto it = find( _lock_map, ptr );

      $assert( it, "lock not found" );

      auto prev = it->counter.sub( 1 );

      if( prev == 1 ) {
        
        if( not is_weak ) it->deleter( ptr );

        _lock_map.erase( it );
      }

      log::lock, ", size2 = ", _lock_map.size(), log::endl;
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



