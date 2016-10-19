#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"


namespace lib {


  TP<TN T0>
  struct owner_ptr;


  TP<TN T0, TN... TT>
  auto make_owner( TT&&... args ) { return owner_ptr< T0 >{ new T0{ forward< T0 >( args )... } }; }


  TP<TN T0>
  struct owner_ptr : nocopy {

    owner_ptr() : _ptr{} { }

    explicit owner_ptr( T0* ptr ) : _ptr{ ptr } { }

    owner_ptr( owner_ptr&& other ) : _ptr { other.release() } { }

    TP<TN U0>
    owner_ptr( owner_ptr< U0 >&& other ) : _ptr { other.release() } { }

    TP<TN U0>
    auto& operator=( owner_ptr< U0 >&& other ) {
      destroy();
      _ptr = other.release();
      return $this;
    }

    auto& operator=( T0* ptr ) {
      destroy();
      _ptr = ptr;
      return $this;
    }

    ~owner_ptr() { destroy(); }

    void destroy() { delete _ptr; }

    T0* release() { return move( _ptr ); }

    auto get() const { return _ptr; }

    auto operator->() { return _ptr; }

    auto const * operator->() const { return _ptr; }

    auto& operator*() { return *_ptr; }

    auto const& operator*() const { return *_ptr; }

    T0* _ptr;

  };

  TP<TN T0>
  struct owner_ptr< T0[] > {

    static_assert( $size( T0 ) == 0, "don't use owner_ptr for arrays, use a vector" );
  };


  TP<TN T0>
  struct out_ref {

    explicit out_ref( T0& value ) : _value{ value } { }

    TP<TN U0>
    out_ref( out_ref< U0 >& other ) : _value { other._value } { }

    TP<TN U0>
    out_ref( out_ref< U0 >&& other ) : out_ref{ other } { }

    auto& get() { return _value; }

    auto operator->() { return &_value; }

    auto& operator*() { return _value; }

    TP<TN U0>
    auto& operator=( U0 value_new ) { 
      
      _value = move( value_new );

      return $this;
    }

    T0& _value;
  };

  TP<TN T0>
  auto make_out( T0& value ) { return out_ref< T0 >{ value }; }

  #define $out( $0 ) make_out( $0 )

}


