#pragma once

#include <cstdio>
#include <new>
#include "macros.h"
#include "types.h"

namespace lib {
  
  TP<TN T0, bool owner = true> 
  struct value : select_t< owner, nocopy, empty_base > {

    static const ssize_t value_size = $size( void*[2] );

    value() : _data{} { }

    value( value&& other ) : _data{ other._data } { }

    value& operator=( value&& other ) { _data = other._data; return $this; }

    ~value() { if( owner ) $this->~T0(); }

    TP<TN U0, TN... TT> 
    static value create( TT&&... args ) {

      $static_assert( $size( U0 ) <= $size( data_ptr ) );

      $static_assert( alignof( T0 ) == alignof( U0 ) );

      auto value0 = value{};

      new( value0._data.get() ) U0{ args... };

      return value0;

    }

    cstr to_string() const { return $this->to_string(); }

    TP<TN... TT>
    auto operator()( TT... args ) { return $this->operator()( args... ); }

    TP<TN... TT>
    auto operator()( TT... args ) const { return $this->operator()( args... ); }

    TP<TN U0> 
    explicit operator U0&() { return *(U0*) _data.get(); }

    TP<TN U0> 
    explicit operator U0 const&() const { return *(U0 const*) _data.get(); }

    auto& operator*() { return *(T0*) _data.get(); }
    auto operator->() { return  (T0*) _data.get(); }

    auto& operator*() const { return *(T0 const*) _data.get(); }
    auto operator->() const { return  (T0 const*) _data.get(); }


    struct data_ptr { 

      char* get() { return ptr; }
      
      char ptr[ value_size ]; 
    };

    alignas( alignof( T0 ) ) data_ptr _data;

  };


}


