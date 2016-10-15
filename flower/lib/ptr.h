#pragma once

#include "macros.h"
#include "types.h"

#include <cstdio>

namespace lib {


  TP<TN T0>
  struct raw_ptr {

    raw_ptr( T0* _ptr ) : _ptr{ _ptr } { }

    TP<TN U0>
    raw_ptr( raw_ptr< U0 >&& other ) : _ptr { move( other._ptr ) } { }

    T0* operator->() { return _ptr; }

    T0 const * operator->() const { return _ptr; }

    T0& operator*() { return *_ptr; }

    T0 const& operator*() const { return *_ptr; }

    T0* _ptr;
  };



  TP<TN T0>
  struct own_ptr : nocopy {

    own_ptr() = default;

    explicit own_ptr( T0* ptr ) : _ptr{ ptr } { }

    own_ptr( own_ptr&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U0>
    own_ptr( own_ptr< U0 >&& other ) : _ptr { move( other._ptr ) } { }

    own_ptr& operator=( own_ptr&& other ) {
      destroy();
      _ptr = move_ptr( other._ptr );
      return $this;
    }

    own_ptr& operator=( T0* ptr ) {
      destroy();
      _ptr = ptr;
      return $this;
    }

    ~own_ptr() { destroy(); }

    void destroy() { delete _ptr; }

    T0* operator->() { return _ptr; }

    T0 const * operator->() const { return _ptr; }

    T0& operator*() { return *_ptr; }

    T0 const& operator*() const { return *_ptr; }

    T0* _ptr{};

  };

  TP<TN T0>
  struct own_ptr< T0[] > {
    static_assert( $size( T0 ) == 0, "don't use own_ptr for arrays, use a vector" );
  };


  TP<TN T0>
  struct out {

    explicit out( T0& value ) : _value{ value } { }

    T0* operator->() { return &_value; }

    T0& operator*() { return _value; }

    T0& _value;
  };

  TP<TN T0>
  out<T0> make_out( T0& value ) { return out< T0 >{ value }; }

  #define $out( $0 ) make_out( $0 )

}


