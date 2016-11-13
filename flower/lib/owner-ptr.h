#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "to-string-selector.h"
#include "value.h"
#include "deleter.h"


namespace lib {


  TP<TN T>
  struct owner_ptr : nocopy {


    owner_ptr( deleter_t del = deleter_default< T >::create() ) : _deleter{ move( del ) } { }

    explicit owner_ptr( T* ptr, deleter_t del = deleter_default< T >::create() ) : 
    
      _ptr{ ptr }, _deleter{ move( del ) } { }

    owner_ptr( owner_ptr&& other ) noexcept : 
    
      _ptr { move( other._ptr ) }, _deleter{ other._deleter } { }

    TP<TN U0>
    owner_ptr( owner_ptr< U0 >&& other ) noexcept : 
    
      _ptr { move( other._ptr ) }, _deleter{ other._deleter } { }

    auto& operator=( T* ptr ) noexcept {

      destroy();

      _ptr = ptr;

      return $this;
    }

    TP<TN U0>
    auto& operator=( owner_ptr< U0 >&& other ) noexcept {

      destroy();

      _ptr = move( other._ptr );

      _deleter = move( other._deleter );

      return $this;
    }

    ~owner_ptr() { destroy(); }

    void destroy() { _deleter( move( _ptr ) ); }

    auto release() { return move( _ptr ); }

    cstr to_string() const { return to_string_selector< T >::to_string( _ptr ); }

    auto get() const { return _ptr; }

    auto operator->() { return _ptr; }

    auto const * operator->() const { return _ptr; }

    auto& operator*() { return *_ptr; }

    auto const& operator*() const { return *_ptr; }

    explicit operator bool() const { return _ptr != nullptr; }


    T _ptr{};
    deleter_t _deleter;
  };


  TP<TN T, TN... TT>
  inline auto make_owner( TT&&... args ) { return owner_ptr< T >{ new T{ forward< TT >( args )... } }; }


}


