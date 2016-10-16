#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"


namespace lib {


  TP<TN T0>
  struct raw_ptr {

    raw_ptr( T0* _ptr ) : _ptr{ _ptr } { }

    TP<TN U0>
    raw_ptr( raw_ptr< U0 >&& other ) : _ptr { move( other._ptr ) } { }

    T0* get() { return _ptr; }
    
    T0* operator->() { return _ptr; }

    T0 const * operator->() const { return _ptr; }

    T0& operator*() { return *_ptr; }

    T0 const& operator*() const { return *_ptr; }

    T0* _ptr;
  };



  TP<TN T0>
  struct owner : nocopy {

    owner() : _ptr{} { }

    explicit owner( T0* ptr ) : _ptr{ ptr } { }

    owner( owner&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U0>
    owner( owner< U0 >&& other ) : _ptr { move( other._ptr ) } { }

    owner& operator=( owner&& other ) {
      destroy();
      _ptr = move( other._ptr );
      return $this;
    }

    owner& operator=( T0* ptr ) {
      destroy();
      _ptr = ptr;
      return $this;
    }

    ~owner() { destroy(); }

    void destroy() { delete _ptr; }

    T0* get() { return _ptr; }

    T0* operator->() { return _ptr; }

    T0 const * operator->() const { return _ptr; }

    T0& operator*() { return *_ptr; }

    T0 const& operator*() const { return *_ptr; }

    T0* _ptr;

  };

  TP<TN T0>
  struct owner< T0[] > {
    static_assert( $size( T0 ) == 0, "don't use owner for arrays, use a vector" );
  };


  TP<TN T0>
  struct out {

    explicit out( T0& value ) : _value{ value } { }

    T0* operator->() { return &_value; }

    T0& operator*() { return _value; }

    T0& _value;
  };

  TP<TN T0>
  out< T0 > make_out( T0& value ) { return out< T0 >{ value }; }

  #define $out( $0 ) make_out( $0 )

}


