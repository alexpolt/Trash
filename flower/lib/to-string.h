#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "buffer.h"

namespace lib {


  TP<TN T0> struct str_printf_format { };

  TP<> struct str_printf_format< char > { constexpr static cstr format = "%c"; };
  TP<> struct str_printf_format< unsigned char > { constexpr static cstr format = "%.2X"; };
  //TP<> struct str_printf_format< cstr > { constexpr static cstr format = "%s"; };
  //TP<> struct str_printf_format< char* > { constexpr static cstr format = "%s"; };
  TP<> struct str_printf_format< bool > { constexpr static cstr format = "%d"; };
  TP<> struct str_printf_format< uint > { constexpr static cstr format = "%u"; };
  TP<> struct str_printf_format< long long > { constexpr static cstr format = "%lli"; };
  TP<> struct str_printf_format< long > { constexpr static cstr format = "%li"; };
  TP<> struct str_printf_format< short > { constexpr static cstr format = "%hi"; };
  TP<> struct str_printf_format< int > { constexpr static cstr format = "%i"; };
  TP<> struct str_printf_format< void* > { constexpr static cstr format = "0x%p"; };
  TP<> struct str_printf_format< float > { constexpr static cstr format = "%+.4f"; };
  TP<> struct str_printf_format< double > { constexpr static cstr format = "%+.4f"; };

  TP<TN T0>
  struct string_format {

    TP<TN U0, char = str_printf_format< U0 >::format[ 0 ]>
    static cstr format( U0 const& arg ) {

      auto& buffer = global::get_buffer< string_format >();

      snprintf( buffer, $array_size( buffer ), 

        str_printf_format< select_t< is_ptr_v< T0 >, void*, T0 > >::format, arg ); 

      return buffer; 
    }
  };


  TP<TN T0, cstr (*)( T0 const& ) = &string_format< T0 >::format>
  inline cstr to_string( T0 const& arg ) { 

    return string_format< T0 >::format( arg );
  }

  TP<TN... TT>
  inline cstr to_string( cstr format, TT... args ) { 

    auto& buffer = global::get_buffer< char >();

    snprintf( buffer, $array_size( buffer ), format, args... ); 

    return buffer; 
  }


}



