#pragma once

#include <cstddef>
#include "macros.h"

namespace lib {

  TP<TN T0, TN T1> 
  constexpr auto max( T0 const& arg0, T1 const& arg1 ) { return arg0 > arg1 ? arg0 : arg1; }

  TP<TN T0, TN T1> 
  constexpr auto min( T0 const& arg0, T1 const& arg1 ) { return arg0 > arg1 ? arg1 : arg0; }

  TP<TN T0, TN T1> 
  constexpr auto less( T0 const* arg0, T1 const* arg1 ) { return  (ptrdiff_t)arg0 < (ptrdiff_t)arg1; }

  TP<TN T0, TN T1> 
  auto find( T0& c, T1 const& value ) { 
    
    auto it_end = end( c );

    for( auto it = begin( c ); it != it_end; it++ ) 

      if( *it == value ) return it;

    return it_end;
  }

}


