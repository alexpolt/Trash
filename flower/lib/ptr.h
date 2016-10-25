#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "locker.h"


namespace lib {


  TP<TN T0, bool is_shared = false>
  struct owner_ptr : nocopy {

    static constexpr bool is_primitive = not is_shared;

    using deleter_t = locker_t::deleter_t;

    owner_ptr() { }

    explicit owner_ptr( T0* ptr ) : _ptr{ ptr } { 

      if( is_shared )

        global::locker< owner_ptr >.lock( _ptr, []( void* ptr ){ delete (T0*) ptr; } );
    }

    TP<TN = enable_if_t< $size( T0 ) and is_shared >>
    explicit owner_ptr( T0* ptr, deleter_t deleter ) : _ptr{ ptr } { 

      global::locker< owner_ptr >.lock( _ptr, deleter );
    }

    owner_ptr( owner_ptr&& other ) noexcept : _ptr { move( other._ptr ) } { }

    TP<TN U0>
    owner_ptr( owner_ptr< U0 >&& other ) noexcept : _ptr { move( other._ptr ) } { }

    TP<TN U0>
    auto& operator=( owner_ptr< U0 >&& other ) noexcept {

      destroy();

      _ptr = move( other._ptr );

      return $this;
    }

    ~owner_ptr() { destroy(); }

    void destroy() { 

      if( is_shared ) global::locker< owner_ptr >.unlock( _ptr );

      else delete _ptr; 
    }

    TP<TN U0, TN = enable_if_t< $size( U0 ) and is_shared >>
    auto lock() { 
     
      $assert( _ptr, "link called on nullptr in shared_ptr" );

      global::locker< owner_ptr >.lock( _ptr );

      return owner_ptr{ _ptr };
    }

    auto release() {       

      return move( _ptr ); 
    }

    auto get() const { return _ptr; }

    auto operator->() { return _ptr; }

    auto const * operator->() const { return _ptr; }

    auto& operator*() { return *_ptr; }

    auto const& operator*() const { return *_ptr; }

    T0* _ptr{};

  };

  TP<TN T0>
  struct owner_ptr< T0[] > {

    static_assert( $size( T0 ) == 0, "don't use owner_ptr for arrays, use a vector" );
  };


  TP<TN T0, TN... TT>
  auto make_owner( TT&&... args ) { return owner_ptr< T0 >{ new T0{ forward< T0 >( args )... } }; }


  TP<TN T0>
  using shared_ptr = owner_ptr< T0, true >;


}


