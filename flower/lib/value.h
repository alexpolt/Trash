#pragma once

#include "new.h"
#include "macros.h"
#include "types.h"
#include "algo.h"
#include "to-string-selector.h"


namespace lib {


  TP<TN T0> 
  struct value : nocopy {

    value() { }

    ~value() { destroy(); }
    
    void destroy() { 
      
      if( $this ) $this->~T0(); 

      _data = data_t{};
    }

    value( value&& other ) : _data{ move( other._data ) } { }

    auto& operator=( value&& other ) { destroy(); _data = move( other._data ); return $this; }

    TP<TN U0, TN... TT> 
    static value create( TT&&... args ) {

      static_assert( $size( U0 ) <= value_size, "The data size is too big for a value<...>.");

      static_assert( alignof( T0 ) == alignof( U0 ), "Alignment is wrong." );

      auto value_new = value{};

      new( value_new._data.data ) U0{ forward< TT >( args )... };

      $clobber(); //get around some strange bug in gcc optimizer that makes value empty

      return move( value_new );
    }

    cstr to_string() const { 

      return to_string_selector< T0 >::to_string( &**this ); 
    }

    TP<TN... TT>
    auto operator()( TT&&... args ) -> decltype( declval< T0& >()( forward< TT >( declval< TT& >() )... ) ) { 

      return $this->operator()( forward< TT > ( args )... ); 
    }

    TP<TN... TT>
    auto operator()( TT&&... args ) const -> decltype( declval< T0& >()( forward< TT >( declval< TT& >() )... ) ) { 

      return $this->operator()( forward< TT > ( args )... ); 
    }

    value get_copy() const { auto v = value{}; v._data = $this._data; return v; }

    T0& operator*() { return type_cast< T0& >( _data.data ); }
    T0* operator->() { return type_cast< T0* >( & _data.data ); }

    T0 const* operator->() const { return type_cast< T0 const* >( & _data.data ); }
    T0 const& operator*() const { return type_cast< T0 const& >( _data.data ); }

    explicit operator bool() const { return * type_cast< void** >( & _data.data ) != nullptr;  }

    TP<TN U, TN = void>
    struct type_size { static constexpr ssize_t value = $size( void*[2] ); };

    TP<TN U>
    struct type_size< U, void_v< U::type_size > > { static constexpr ssize_t value = U::type_size; };

    TP<TN U>
    static constexpr ssize_t type_size_v = type_size< U >::value;

    static constexpr ssize_t value_size = type_size_v< T0 >;

    alignas( alignof( T0 ) )
    struct data_t {

      static constexpr bool is_primitive = true;

      char data[ value_size ]; 

    } _data{};
  };


}


