#pragma once

#include <cstring>
#include <cstdio>
#include <new>
#include "macros.h"
#include "types.h"

namespace lib {
  
  TP<TN T0, bool is_owner = false> 
  struct value : select_t< is_owner, nocopy, empty_struct > {

    static constexpr bool is_primitive = not is_owner;

    static constexpr ssize_t value_size = $size( void*[2] );


    TP<TN U0, TN... TT> 
    static value create( TT&&... args ) {

      static_assert( $size( U0 ) <= $size( data_ptr ), "The data size is too big for a value<...>.");

      static_assert( alignof( T0 ) == alignof( U0 ), "Alignment is wrong." );

      auto value_new = value{};

      new( type_cast< void* >( & value_new._data ) ) U0{ forward< TT >( args )... };

      return value_new;

    }

    cstr to_string() const { return $this->to_string(); }

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


    struct data_ptr { 

      data_ptr() : _ptr{} { }

      ~data_ptr() { 

        auto ptr = type_cast< ssize_t* >( & _ptr );

        if( is_owner and *ptr != 0 ) 

            type_cast< T0* >( & _ptr )->~T0(); 
      }

      data_ptr( data_ptr const& other ) noexcept { 

        memcpy( _ptr, other._ptr, value_size ); 
      }

      data_ptr( data_ptr&& other ) noexcept { 

        memcpy( _ptr, other._ptr, value_size ); 
        memset( other._ptr, 0, value_size );
      }

      auto& operator=( data_ptr const& other ) noexcept {

        memcpy( _ptr, other._ptr, value_size ); 

        return $this;
      }

      auto& operator=( data_ptr&& other ) noexcept {

        memcpy( _ptr, other._ptr, value_size ); 
        memset( other._ptr, 0, value_size );

        return $this;
      }

      char* get() { return _ptr; }

      char const* get() const { return _ptr; }
      
      char _ptr[ value_size ]; 
    };


    alignas( alignof( T0 ) ) data_ptr _data;

  };

  TP<TN T0> 
  using owner = value< T0, true >;

}


