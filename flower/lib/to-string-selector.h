#pragma once


#include "macros.h"
#include "types.h"
#include "to-string.h"

namespace lib {


  TP<TN T0, TN = void> 
  struct to_string_selector {

    static cstr to_string( T0 const* ptr ) { 

      return lib::to_string( "ptr( 0x%p )", ptr ); 
    }
  };


  TP<TN T0> 
  struct to_string_selector< T0, void_t< decltype( (cstr (T0::*)() const) &T0::to_string ) >> {

    static cstr to_string( T0 const* ptr ) { 

      return ptr->to_string(); 
    }
  };


}



