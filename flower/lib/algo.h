#pragma once

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


  TP<TN T0, TN T1, TN = disable_if_t< is_primitive_class_v< T0 > and is_primitive_class_v< T1 >>, TN = void> 
  constexpr bool equal( T0 const& arg0, T1 const& arg1 ) { return arg0 == arg1; }

  TP<TN T0, TN T1, TN = enable_if_t< is_primitive_class_v< T0 > and is_primitive_class_v< T1 > >> 
  bool equal( T0 const& arg0, T1 const& arg1 ) {

    return memcmp( &arg0, &arg1, $size( arg0 ) ) == 0;
  }

  bool equal( cstr arg0, cstr arg1 ) { return strcmp( arg0, arg1 ) == 0; }
  bool equal( char* arg0, char* arg1 ) { return strcmp( arg0, arg1 ) == 0; }


  TP<TN T0, TN T1> 
  constexpr bool less( T0 const* arg0, T1 const* arg1 ) { return  (ssize_t)arg0 < (ssize_t)arg1; }


  TP<TN T0, TN T1, TN = disable_if_t< is_ref_v< T1 > >, decltype( &T1::operator() ) = &T1::operator()>
  auto find( T0& c, T1&& fn ) { 
    
    auto it_end = end( c );

    for( auto it = begin( c ); it != it_end; it++ ) 

      if( fn( *it ) ) return it;

    return it_end;
  }

  TP<TN T0, TN T1 >
  auto find( T0& c, T1 const& value, char = 0 ) { 
    
    auto it_end = end( c );

    for( auto it = begin( c ); it != it_end; it++ ) 

      if( equal( *it, value ) ) return it;

    return it_end;
  }

}


