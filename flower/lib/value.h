#pragma once

#include <cstdio>
#include "macros.h"
#include <new>


namespace lib {
  
  $T<$N T0> struct value {
  //$T<$N T0, owner> struct value {

    ~value() { $this->~T0(); }


    $T<$N U0, $N... TT> 
    static auto create( TT&&... args ) {

      $static_assert( $size( U0 ) <= $size( _data ) );
      $static_assert( alignof( T0 ) == alignof( U0 ) );

      auto v = value< T0 >{};

      new( v._data ) U0{ args... };

      return v;

    }

    $T<$N... TT>
    auto operator()( TT... args ) { return $this->operator()( args... ); }

    $T<$N... TT>
    auto operator()( TT... args ) const { return $this->operator()( args... ); }

    $T<$N U0> 
    explicit operator U0&() { return *( U0* ) _data; }

    $T<$N U0> 
    explicit operator U0 const&() const { return *( U0* ) _data; }


    T0& operator*() { return ( T0& ) _data; }
    T0* operator->() { return ( T0* ) _data; }

    T0 const& operator*() const { return ( T0 const& ) _data; }
    T0 const* operator->() const { return ( T0 const* ) _data; }

    using data_t = void*[2];

    alignas( alignof( T0 ) ) data_t _data;

  };


}

