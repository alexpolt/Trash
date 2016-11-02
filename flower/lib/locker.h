#pragma once

#include "macros.h"
#include "types.h"
#include "assert.h"
#include "atomic.h"
#include "hash-map.h"
#include "to-string.h"
#include "log.h"

namespace lib {


  struct locker_t : nocopy {

    using deleter_t = void (*)( void* );

    locker_t() {

      _lock_map.reserve( 32 );
    }

    ~locker_t() {

      $assert( _lock_map.size() == 0, "lock_map size is not zero");
    }

    void lock( void* ptr, deleter_t deleter ) {

      log::lock, "lock( ", ptr;

      auto it = _lock_map[ ptr ];

      int counter = 1;

      if( it )

        counter = it->counter.add( 1 ) + 1;

      else

        _lock_map.insert( ptr, lock_node{ ptr, counter, deleter } );

      log::lock, ", ", counter, " )", log::endl;
    }

    void lock( void* ptr ) {

      log::lock, "lock( ", ptr;

      auto it = _lock_map[ ptr ];

      $assert( it, "lock not found" ); 

      auto counter = it->counter.add( 1 ) + 1;

      log::lock, ", ", counter, " )", log::endl;
    }

    bool unlock( void* ptr, bool is_weak = false ) {

      auto it = _lock_map[ ptr ];

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

      auto it = _lock_map[ ptr ];

      if( it ) 

        it->counter.load();

      return 0;
    }

    struct lock_node {

      lock_node() { }

      lock_node( void* p, int c, deleter_t d ) : counter{}, deleter{ d } { 

        counter = c;
      }

      lock_node( lock_node const& other ) : counter{}, deleter{ other.deleter } { 

        counter = other.counter.load();
      }

      lock_node( lock_node&& other ) : counter{}, deleter{ move( other.deleter ) } { 

        counter = other.counter.load();
        
        other.counter = 0;
      }

      lock_node& operator=( lock_node const& other ) {

        deleter = other.deleter;
        counter = other.counter.load();

        return $this;
      }

      cstr to_string() const { return lib::to_string( "lock_node( %d )", counter.load() ); }

      atomic< int > counter{};
      deleter_t deleter{};
    };


    hash_map< void*, lock_node > _lock_map;
  };


  namespace global {

    TP<TN T0>
    locker_t locker{};
  }


}



