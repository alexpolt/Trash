#pragma once

#include "macros.h"
#include "types.h"
#include "ret.h"
#include "value.h"


namespace lib {


  TP<TN T0>
  struct optional;

  TP<TN T0, TN... TT>
  auto make_optional( TT&&... args ) { return optional< T0 >{ forward< T0 >( args )... }; }


  TP<TN T0>
  struct optional {
    
    using value_type = T0;

    optional() : _ret{ ret_t::failed } { }

    optional( ret_t e ) : _ret{ e  } { }

    optional( value_type&& v ) {
        
      _object = value< T0 >::template create< T0 >( move( v ) );

      _ret = ret_t::success;
    }

    TP<TN... TT>
    explicit optional( TT... args ) {
        
      _object = value< T0 >::template create< T0 >( forward< TT >( args )... );

      _ret = ret_t::success;
    }

    auto& operator*() { return *_object; }
    auto& operator*() const { return *_object; }
    auto  operator->() { return *_object; }
    auto  operator->() const { return *_object; }

    explicit operator bool() const { return _ret == ret_t::success; }

    value< T0 > _object;
    ret_t _ret;
  };


}



