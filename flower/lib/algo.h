#pragma once

#include <cstddef>
#include <cstring>
#include "macros.h"
#include "types.h"

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


  TP<TN T0, TN T1, TN = enable_if_t< not ( is_primitive_v< T0 > or is_primitive_v< T1 > ) >, TN = void> 
  constexpr bool equal( T0 const& arg0, T1 const& arg1 ) { return arg0 == arg1; }

  TP<TN T0, TN T1, TN = enable_if_t< is_primitive_v< T0 > and is_primitive_v< T1 > >, TN = void> 
  constexpr bool equal( T0 const& arg0, T1 const& arg1, ... ) { return arg0 == arg1; }

  TP<TN T0, TN T1, TN = enable_if_t< is_primitive_class_v< T0 > and is_primitive_class_v< T1 > >> 
  bool equal( T0 const& arg0, T1 const& arg1 ) {

    return not memcmp( &arg0, &arg1, $size( arg0 ) );
  }

  bool equal( cstr arg0, cstr arg1 ) { return not strcmp( arg0, arg1 ); }
  bool equal( char* arg0, char* arg1 ) { return not strcmp( arg0, arg1 ); }


  TP<TN T0, TN T1> 
  constexpr bool less( T0 const* arg0, T1 const* arg1 ) { return  (ptrdiff_t)arg0 < (ptrdiff_t)arg1; }


  TP<TN T0, TN T1> 
  auto find( T0& c, T1 const& value ) { 
    
    auto it_end = end( c );

    for( auto it = begin( c ); it != it_end; it++ ) 

      if( *it == value ) return it;

    return it_end;
  }

}


