#pragma once

#include <cstdio>
#include "macros.h"
#include <new>


namespace lib {
  
  $T<$N T0> struct value {
  //$T<$N T0, owner> struct value {

    ~value() { $->~T0(); }


    $T<$N U0, $N... TT> 
    static auto create( TT&&... args ) {

      $static_assert( sizeof( U0 ) <= sizeof( data ) );
      $static_assert( alignof( T0 ) == alignof( U0 ) );

      auto v = value< T0 >{};

      new( v.data ) U0{ args... };

      return v;

    }

    $T<$N... TT>
    auto operator()( TT... args ) { return $->operator()( args... ); }

    $T<$N U0> 
    explicit operator U0&() { 
      printf("cast\n");
      return *( U0* ) data; 
    }


    T0& operator*() { return ( T0& ) data; }
    T0* operator->() { return ( T0* ) data; }

    T0 const& operator*() const { return ( T0 const& ) data; }
    T0 const* operator->() const { return ( T0 const* ) data; }

    using data_t = void*[2];

    alignas( alignof( T0 ) ) data_t data[ 2 ];

  };


}

