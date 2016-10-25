#pragma once

#include <new>
#include <cstddef>
#include "macros.h"
#include "types.h"
#include "algo.h"
#include "to-string-selector.h"

namespace lib {
  
  TP<TN T0> 
  struct value : nocopy {

    static constexpr ssize_t value_size = max( $size( T0 ), $size( void*[2] ) );

    value() { }

    ~value() { if( *type_cast< ptrdiff_t* >( &_data ) ) $this->~T0(); }

    value( value&& other ) : _data{ move( other._data ) } { }

    auto& operator=( value&& other ) { _data = move( other._data ); return $this; }

    TP<TN U0, TN... TT> 
    static value create( TT&&... args ) {

      static_assert( $size( U0 ) <= value_size, "The data size is too big for a value<...>.");

      static_assert( alignof( T0 ) == alignof( U0 ), "Alignment is wrong." );

      auto value_new = value{};

      new( type_cast< void* >( & value_new._data ) ) U0{ forward< TT >( args )... };

      return value_new;

    }

    cstr to_string() const { return to_string_selector< T0 >::to_string( &$this ); }

    TP<TN... TT>
    auto operator()( TT... args ) const -> decltype( declval< T0& >()( forward< TT >( declval< TT >() )... ) ) { 

      return $this->operator()( forward< TT > ( args )... ); 
    }

    TP<TN U0> 
    explicit operator U0&() { return type_cast< U0& >( _data ); }

    TP<TN U0> 
    explicit operator U0&() const { return type_cast< U0 const& >( _data ); }

    auto& operator*() { return type_cast< T0& >( _data ); }
    auto operator->() { return type_cast< T0* >( & _data ); }

    auto operator->() const { return type_cast< T0 const* >( & _data ); }
    auto& operator*() const { return type_cast< T0 const& >( _data ); }

    struct data_t {

      static constexpr bool is_primitive = true;

      char data[ value_size ]; 

    } _data{};
  };


}


