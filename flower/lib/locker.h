#pragma once

#include "macros.h"
#include "types.h"
#include "assert.h"
#include "atomic.h"
#include "hash-map.h"
#include "to-string.h"
#include "log.h"
#include "alloc-default.h"


namespace lib {


  struct locker : nocopy {


    using deleter_t = void (*)( void* );
    using allocator = value< allocator >;

    static allocator create_alloc() { return alloc_default::create( "lock_map" ); }


    locker() { }

    ~locker() {

      ssize_t counter_s = 0, counter_w = 0;

      for( auto& e : _lock_map ) {
        
        counter_s += e.counter_s.load();
        counter_w += e.counter_w.load();
      }
      
      $assert( counter_s == 0, "some objects are still were not destroyed" );

      if( counter_w )

        log::warn, $file_line "weak counter > 0, total weak increments = ", counter_w, log::endl;
    }

    void lock( void* ptr, deleter_t deleter, bool is_weak, cstr name ) {

      if( _lock_map.size() == 0 ) _lock_map.reserve( 16 );

      log::lock, "new ";

      auto it = _lock_map[ ptr ];

      if( not it )

        _lock_map.insert( ptr, lock_node{ 0, 0, deleter, name } );

      else if( deleter and it->deleter ) 

        $assert( false, "deleter is already set" );

      lock( ptr, is_weak );
    }

    void lock( void* ptr, bool is_weak ) {

      auto it = _lock_map[ ptr ];

      $assert( it, "lock not found" ); 

      int counter_s = not is_weak, counter_w = is_weak;

      if( not is_weak ) {
        
        counter_s = it->counter_s.add( counter_s ) + counter_s;
        counter_w = it->counter_w.load();

      } else {

        counter_s = it->counter_s.load();
        counter_w = it->counter_w.add( counter_w ) + counter_w;
      }

      log::lock, it->name, " lock_", ( is_weak ? "w" : "s" ), "( ", ptr;
      log::lock, " ), s = ", counter_s, ", w = ", counter_w, log::endl;
    }

    bool unlock( void* ptr, bool is_weak ) {
 
      auto it = _lock_map[ ptr ];

      $assert( it, "lock not found" );

      int counter_s = not is_weak, counter_w = is_weak;

      if( not is_weak ) {
        
        counter_s = it->counter_s.sub( counter_s ) - counter_s;
        counter_w = it->counter_w.load();

      } else {

        counter_s = it->counter_s.load();
        counter_w = it->counter_w.sub( counter_w ) - counter_w;
      }

      log::lock, it->name, " unlock_", ( is_weak ? "w" : "s" ), "( ", ptr;
      log::lock, " ), s = ", counter_s, ", w = ", counter_w, log::endl;

      if( counter_s == 0 ) {
        
        if( it->deleter ) it->deleter( ptr );

        if( counter_w > 0 ) 
          
          log::warn, "object destroyed, but weak counter = ", counter_w, log::endl;

        else

          _lock_map.erase( it );

        return true;
      }

      return false;
    }

    int use_count( void* ptr ) { 

      auto it = _lock_map[ ptr ];

      if( it ) 

        return it->counter_s.load() + it->counter_w.load();

      return 0;
    }

    bool expired( void* ptr ) { 

      auto it = _lock_map[ ptr ];

      if( it ) 

        return it->counter_s.load() == 0;

      return true;
    }

    struct lock_node {

      lock_node() { }

      lock_node( int s, int w, deleter_t d, cstr n ) : 
        counter_s{}, counter_w{}, deleter{ d }, name{ n } { 

        counter_s = s;
        counter_w = w;
      }

      lock_node( lock_node const& other ) : 
        counter_s{}, counter_w{}, deleter{ other.deleter }, name{ other.name } { 

        counter_s = other.counter_s.load();
        counter_w = other.counter_w.load();
      }

      lock_node( lock_node&& other ) : 
        counter_s{}, counter_w{}, deleter{ move( other.deleter ) }, name{ move( other.name ) } { 

        counter_s = other.counter_s.load();
        counter_w = other.counter_w.load();
        
        other.counter_s = 0;
        other.counter_w = 0;
      }

      lock_node& operator=( lock_node const& other ) {

        counter_s = other.counter_s.load();
        counter_w = other.counter_w.load();
        deleter = other.deleter;
        name = other.name;

        return $this;
      }

      cstr to_string() const { 
        
        return lib::to_string( "lock_node %s( s = %d, w = %d, d = %p )", 
                                  name, counter_s.load(), counter_w.load(), (void*) deleter );
      }

      atomic< int > counter_s{};
      atomic< int > counter_w{};
      deleter_t deleter{};
      cstr name{};
    };


    hash_map< void*, lock_node > _lock_map{ create_alloc() };
  };



}



