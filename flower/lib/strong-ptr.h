#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "global.h"
#include "to-string-selector.h"


namespace lib {


  TP<TN T0, bool is_weak = false>
  struct strong_ptr : nocopy {

    using deleter_t = locker::deleter_t;

    strong_ptr() { }

    explicit strong_ptr( T0* ptr, cstr name = "noname" ) : _ptr{ ptr } { 

      deleter_t deleter = []( void* ptr ){ delete (T0*) ptr; };

      if( is_weak ) deleter = nullptr;

      get_locker().lock( _ptr, deleter, is_weak, name );
    }

    strong_ptr( T0* ptr, deleter_t deleter, cstr name = "noname" ) : _ptr{ ptr } { 

      get_locker().lock( _ptr, deleter, is_weak, name );
    }

    strong_ptr( strong_ptr&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U>
    strong_ptr( strong_ptr< U, is_weak >&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U, bool weak, TN = enable_if_t< $size( U ) and is_weak >>
    strong_ptr( strong_ptr< U, weak > const& other ) : _ptr { other._ptr } { 

      get_locker().lock( _ptr, is_weak );
    }

    TP<TN U>
    auto& operator=( strong_ptr< U, is_weak >&& other ) {

      destroy();

      _ptr = move( other._ptr );

      return $this;
    }

    TP<TN U, bool weak, TN = enable_if_t< $size( U ) and is_weak >>
    auto& operator=( strong_ptr< U, weak > const& other ) {

      destroy();

      _ptr = other._ptr;

      get_locker().lock( _ptr, is_weak );

      return $this;
    }


    ~strong_ptr() { destroy(); }

    void destroy() { 

      if( _ptr ) {
        
        bool deleted = get_locker().unlock( _ptr, is_weak );

        if( deleted ) _ptr = nullptr;
      }
    }

    static auto& get_locker() { return global::lock_map<>; }

    auto lock() const { 
     
      $assert( _ptr, "lock called on nullptr in strong_ptr" );

      get_locker().lock( _ptr, is_weak );

      strong_ptr ptr;

      ptr._ptr = _ptr;

      return move( ptr );
    }

    auto reset() { destroy(); }

    int use_count() { return get_locker().use_count( _ptr ); }

    bool expired() { return get_locker().expired( _ptr ); }

    cstr to_string() const { return to_string_selector< T0 >::to_string( _ptr ); }

    auto get() const { return _ptr; }

    T0* operator->() { return _ptr; }

    T0 const * operator->() const { return _ptr; }

    T0& operator*() { return *_ptr; }

    T0 const& operator*() const { return *_ptr; }

    explicit operator bool() const { return _ptr != nullptr; }


    T0* _ptr{};
  };

  TP<TN T0>
  struct strong_ptr< T0[] > {

    static_assert( $size( T0 ) == 0, "don't use strong_ptr for arrays, use a vector" );
  };


  TP<TN T0>
  using weak_ptr = strong_ptr< T0, true >;


  TP<TN T0, TN... TT>
  inline auto make_strong( TT&&... args ) { return strong_ptr< T0 >{ new T0{ forward< TT >( args )... } }; }


  TP<TN T0, bool is_weak0, bool is_weak1>
  bool operator==( strong_ptr< T0, is_weak0 > const& l, strong_ptr< T0, is_weak1 > const& r ) {

    return l.get() == r.get();
  }

  TP<TN T0, bool is_weak0>
  bool operator==( T0 const* l, strong_ptr< T0, is_weak0 > const& r ) {

    return l == r.get();
  }

  TP<TN T0, bool is_weak0>
  bool operator==( strong_ptr< T0, is_weak0 > const& l, T0 const* r ) {

    return l.get() == r;
  }



}



