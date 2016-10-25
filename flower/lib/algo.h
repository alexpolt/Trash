#pragma once

#include <cstddef>
#include "macros.h"

namespace lib {

  TP<TN T0> 
  constexpr auto& max( T0 const& arg0 ) { return arg0; }

  TP<TN T0, TN... TT> 
  constexpr auto& max( T0 const& arg0, TT const&... args ) { 

    auto& r = max( args... );

    return arg0 > r ? arg0 : r; 
  }

  TP<TN T0> 
  constexpr auto& min( T0 const& arg0 ) { return arg0; }

  TP<TN T0, TN... TT> 
  constexpr auto& min( T0 const& arg0, TT const&... args ) { 

    auto& r = min( args... );

    return arg0 > r ? r : arg0; 
  }

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


