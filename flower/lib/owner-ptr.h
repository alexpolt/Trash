#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "to-string-selector.h"
#include "value.h"


namespace lib {


  TP<TN T>
  struct owner_ptr : nocopy {

    using value_type = no_array_t< T >;
    using pointer = value_type*;
    using const_pointer = value_type const*;

    owner_ptr() { }

    explicit owner_ptr( pointer ptr ) : _ptr{ ptr } { }

    owner_ptr( owner_ptr&& other ) : _ptr { move( other._ptr ) } { }

    TP<TN U, TN = enable_if_t< is_base_v< T, U > >>
    owner_ptr( owner_ptr< U >&& other ) : _ptr { move( other._ptr ) } { }

    auto& operator=( pointer ptr ) noexcept {

      destroy();

      _ptr = ptr;

      return $this;
    }

    TP<TN U, TN = enable_if_t< is_base_v< T, U > >>
    auto& operator=( owner_ptr< U >&& other ) noexcept {

      destroy();

      _ptr = move( other._ptr );

      return $this;
    }

    ~owner_ptr() { destroy(); }

    void destroy() { 

      if( $this ) {

        if( not is_array_v< T > ) 

             delete _ptr; 

        else delete[] _ptr;
      }
    }

    auto release() { return move( _ptr ); }

    cstr to_string() const { return to_string_selector< T >::to_string( _ptr ); }

    auto get() const { return _ptr; }

    pointer operator->() { return _ptr; }

    const_pointer operator->() const { return _ptr; }

    auto& operator*() { return *_ptr; }

    auto const&operator*() const { return *_ptr; }

    auto& operator[]( ssize_t index ) { return _ptr[ index ]; }

    auto const& operator[]( ssize_t index ) const { return _ptr[ index ]; }

    explicit operator bool() const { return _ptr != nullptr; }

    TP<TN U, TN = disable_if_t< is_ref_v< U > >>
    explicit operator owner_ptr< U >() { 
      
      auto ptr = release(); 
      
      return owner_ptr< U >{ static_cast< U* >( ptr ) };
    }

    pointer _ptr{};
  };


  TP<TN T, TN... TT>
  inline auto make_owner( TT&&... args ) { return owner_ptr< T >{ new T{ forward< TT >( args )... } }; }


}


