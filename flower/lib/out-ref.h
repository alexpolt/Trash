#pragma once

#include "macros.h"
#include "types.h"


namespace lib {


  TP<TN T0>
  struct out_ref {

    explicit out_ref( T0& value ) : _value{ value } { }

    TP<TN U0>
    out_ref( out_ref< U0 >& other ) : _value { other._value } { }

    TP<TN U0>
    out_ref( out_ref< U0 >&& other ) : _value{ other._value } { }

    auto& get() { return _value; }

    auto operator->() { return &_value; }

    auto& operator*() { return _value; }

    TP<TN U0>
    auto& operator=( U0 value_new ) { 
      
      _value = move( value_new );

      return $this;
    }

    T0& _value;
  };

  TP<TN T0>
  auto make_out( T0& value ) { return out_ref< T0 >{ value }; }

  #define $out( $0 ) make_out( $0 )

}


