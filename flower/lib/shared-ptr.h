#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "locker.h"
#include "to-string-selector.h"

namespace lib {

  struct locker_default_tag {};


  TP<TN T0, bool is_weak = false>
  struct shared_ptr : nocopy {

    using deleter_t = locker_t::deleter_t;

    shared_ptr() { }

    explicit shared_ptr( T0* ptr ) : _ptr{ ptr } { 

      get_locker().lock( _ptr, []( void* ptr ){ delete (T0*) ptr; } );
    }

    explicit shared_ptr( T0* ptr, deleter_t deleter ) : _ptr{ ptr } { 

      get_locker().lock( _ptr, deleter );
    }

    shared_ptr( shared_ptr&& other ) noexcept : _ptr { move( other._ptr ) } { }

    TP<TN U0, bool weak>
    shared_ptr( shared_ptr< U0, weak >&& other ) noexcept : _ptr { move( other._ptr ) } { }

    TP<TN U0, bool weak>
    auto& operator=( shared_ptr< U0, weak >&& other ) noexcept {

      destroy();

      _ptr = move( other._ptr );

      return $this;
    }

    ~shared_ptr() { destroy(); }

    void destroy() { 

      if( _ptr ) {
        
        bool deleted = get_locker().unlock( _ptr, is_weak );

        if( deleted ) _ptr = nullptr;
      }
    }

    TP<TN U0, TN = void>
    struct locker_tag { using type = locker_default_tag; };

    TP<TN U0>
    struct locker_tag< U0, void_t< typename U0::locker_tag > > { using type = typename U0::locker_tag; };

    static auto& get_locker() { return global::locker< typename locker_tag< T0 >::type >; }

    auto lock() { 
     
      $assert( _ptr, "lock called on nullptr in shared_ptr" );

      get_locker().lock( _ptr );

      shared_ptr ptr;

      ptr._ptr = _ptr;

      return ptr;
    }

    auto reset() { destroy(); }

    int use_count() { return get_locker().use_count( _ptr ); }

    cstr to_string() const { return to_string_selector< T0 >::to_string( _ptr ); }

    auto get() const { return _ptr; }

    T0* operator->() { return _ptr; }

    T0 const * operator->() const { return _ptr; }

    T0& operator*() { return *_ptr; }

    T0 const& operator*() const { return *_ptr; }

    explicit operator bool() const { return _ptr != nullptr; }


    T0* _ptr{};
  };

  TP<TN T0>
  struct shared_ptr< T0[] > {

    static_assert( $size( T0 ) == 0, "don't use shared_ptr for arrays, use a vector" );
  };

  TP<TN T0>
  using weak_ptr = shared_ptr< T0, true >;

  TP<TN T0, TN... TT>
  inline auto make_shared( TT&&... args ) { return shared_ptr< T0 >{ new T0{ forward< T0 >( args )... } }; }



}


