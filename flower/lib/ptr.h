#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"


namespace lib {


  TP<TN T0>
  struct owner : nocopy {

    owner() : _ptr{} { }

    explicit owner( T0* ptr ) : _ptr{ ptr } { }

    owner( owner&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U0>
    owner( owner< U0 >&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U0>
    auto& operator=( owner< U0 >&& other ) {
      destroy();
      _ptr = move( other._ptr );
      return $this;
    }

    auto& operator=( T0* ptr ) {
      destroy();
      _ptr = ptr;
      return $this;
    }

    ~owner() { destroy(); }

    void destroy() { delete _ptr; }

    auto get() const { return _ptr; }

    auto operator->() { return _ptr; }

    auto const * operator->() const { return _ptr; }

    auto& operator*() { return *_ptr; }

    auto const& operator*() const { return *_ptr; }

    T0* _ptr;

  };

  TP<TN T0>
  struct owner< T0[] > {
    static_assert( $size( T0 ) == 0, "don't use owner for arrays, use a vector" );
  };


  TP<TN T0>
  struct out {

    explicit out( T0& value ) : _value{ value } { }

    TP<TN U0>
    out( out< U0 >& other ) : _value { other._value } { }

    TP<TN U0>
    out( out< U0 >&& other ) : out{ other } { }

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
  auto make_out( T0& value ) { return out< T0 >{ value }; }

  #define $out( $0 ) make_out( $0 )

}


