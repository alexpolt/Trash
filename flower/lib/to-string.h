#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "global.h"

namespace lib {

  TP<TN T0> struct str_format { };

  TP<> struct str_format< char > { constexpr static cstr format = "%c"; };
  TP<> struct str_format< cstr > { constexpr static cstr format = "%s"; };
  TP<> struct str_format< char* > { constexpr static cstr format = "%s"; };
  TP<> struct str_format< bool > { constexpr static cstr format = "%d"; };
  TP<> struct str_format< uint > { constexpr static cstr format = "%u"; };
  TP<> struct str_format< long long > { constexpr static cstr format = "%lli"; };
  TP<> struct str_format< long > { constexpr static cstr format = "%li"; };
  TP<> struct str_format< short > { constexpr static cstr format = "%hi"; };
  TP<> struct str_format< int > { constexpr static cstr format = "%i"; };
  TP<> struct str_format< void* > { constexpr static cstr format = "%p"; };
  TP<> struct str_format< float > { constexpr static cstr format = "%.5f"; };
  TP<> struct str_format< double > { constexpr static cstr format = "%.5f"; };

  TP<TN T0>
  cstr to_string( T0 const& arg ) { 

    auto& buffer = global::get_buffer<char>();

    snprintf( buffer, $length( buffer ), str_format< T0 >::format, arg ); 

    return buffer; 
  }

}

