#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "to-string-selector.h"


namespace lib {


  TP<TN T0>
  struct owner_ptr : nocopy {


    owner_ptr() { }

    explicit owner_ptr( T0* ptr ) : _ptr{ ptr } { }

    owner_ptr( owner_ptr&& other ) noexcept : _ptr { move( other._ptr ) } { }

    TP<TN U0>
    owner_ptr( owner_ptr< U0 >&& other ) noexcept : _ptr { move( other._ptr ) } { }

    auto& operator=( T0* ptr ) noexcept {

      destroy();

      _ptr = ptr;

      return $this;
    }

    TP<TN U0>
    auto& operator=( owner_ptr< U0 >&& other ) noexcept {

      destroy();

      _ptr = move( other._ptr );

      return $this;
    }

    ~owner_ptr() { destroy(); }

    void destroy() { 

      delete _ptr; 
    }

    auto release() {       

      return move( _ptr ); 
    }

    cstr to_string() const { return to_string_selector< T0 >::to_string( _ptr ); }

    auto get() const { return _ptr; }

    auto operator->() { return _ptr; }

    auto const * operator->() const { return _ptr; }

    auto& operator*() { return *_ptr; }

    auto const& operator*() const { return *_ptr; }

    T0* _ptr{};

  };

  TP<TN T0>
  struct owner_ptr< T0[] > {

    static_assert( $size( T0 ) == 0, "don't use owner_ptr for arrays, use a vector" );
  };


  TP<TN T0, TN... TT>
  inline auto make_owner( TT&&... args ) { return owner_ptr< T0 >{ new T0{ forward< T0 >( args )... } }; }


}


