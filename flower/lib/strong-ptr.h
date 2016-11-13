#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "lock-map.h"
#include "to-string-selector.h"
#include "value.h"
#include "deleter.h"


namespace lib {


  TP<TN T, bool is_weak = false>
  struct strong_ptr : nocopy {

    using value_type = no_array_t< T >;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using reference = value_type&;
    using const_reference = value_type const&;

    strong_ptr() { }

    explicit strong_ptr( pointer ptr, cstr name = "noname" ) : 
    
      strong_ptr{ ptr, is_weak ? deleter_t{} : deleter_default< T >::create(), name } { }

    strong_ptr( pointer ptr, deleter_t deleter, cstr name = "noname" ) : _ptr{ ptr } { 

      $assert( ptr, "null pointer is not allowed" );

      get_locker().lock( _ptr, move( deleter ), is_weak, name );
    }

    strong_ptr( strong_ptr&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U, TN = enable_if_t< is_base_v< T, U > >>
    strong_ptr( strong_ptr< U, is_weak >&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U, bool weak, TN = enable_if_t< is_base_v< T, U > and is_weak >>
    strong_ptr( strong_ptr< U, weak > const& other ) : _ptr { other._ptr } { 

      get_locker().lock( _ptr, is_weak );
    }

    TP<TN U, TN = enable_if_t< is_base_v< T, U > >>
    auto& operator=( strong_ptr< U, is_weak >&& other ) {

      destroy();

      _ptr = move( other._ptr );

      return $this;
    }

    TP<TN U, bool weak, TN = enable_if_t< is_base_v< T, U > and is_weak >>
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

    cstr to_string() const { return to_string_selector< T >::to_string( _ptr ); }

    auto get() const { return _ptr; }

    pointer operator->() { return _ptr; }

    const_pointer operator->() const { return _ptr; }

    auto& operator*() { return *_ptr; }

    auto const& operator*() const { return *_ptr; }

    auto& operator[]( ssize_t index ) { return _ptr[ index ]; }

    auto const& operator[]( ssize_t index ) const { return _ptr[ index ]; }

    explicit operator bool() const { return _ptr != nullptr; }

    TP<TN U, TN = disable_if_t< is_ref_v< U > >>
    explicit operator strong_ptr< U >() { 

      if( not $this ) return strong_ptr< U >{};

      return strong_ptr< U >{ static_cast< U* >( _ptr ), deleter_t{} };
    }

    pointer _ptr{};
  };


  TP<TN T>
  using weak_ptr = strong_ptr< T, true >;


  TP<TN T, TN... TT>
  inline auto make_strong( TT&&... args ) { return strong_ptr< T >{ new T{ forward< TT >( args )... } }; }


  TP<TN T, bool is_weak0, bool is_weak1>
  bool operator==( strong_ptr< T, is_weak0 > const& l, strong_ptr< T, is_weak1 > const& r ) {

    return l.get() == r.get();
  }

  TP<TN T, bool is_weak0>
  bool operator==( T const* l, strong_ptr< T, is_weak0 > const& r ) {

    return l == r.get();
  }

  TP<TN T, bool is_weak0>
  bool operator==( strong_ptr< T, is_weak0 > const& l, T const* r ) {

    return l.get() == r;
  }



}



