#pragma once

#include <cstdio>
#include "macros.h"
#include "types.h"
#include "value.h"

namespace log {


  $T<$N T0> 
  struct log_t : T0 { };
  
  struct endl_t { };

  struct printer_t {
    $T<$N... TT>
    static void print( TT... args ) { printf( args... ); }
    static void print( endl_t ) { printf( "\n" ); }
  };

  log_t< printer_t > info;
  log_t< printer_t > error;
  endl_t endl;


  $T<$N T0> struct log_format { };

  $T<> struct log_format< cstr > { constexpr static cstr format = "%s"; };
  $T<> struct log_format< char* > { constexpr static cstr format = "%s"; };
  $T<> struct log_format< bool > { constexpr static cstr format = "%d"; };
  $T<> struct log_format< uint > { constexpr static cstr format = "%d"; };
  $T<> struct log_format< int > { constexpr static cstr format = "%d"; };
  $T<> struct log_format< float > { constexpr static cstr format = "%.5f"; };
  $T<> struct log_format< double > { constexpr static cstr format = "%.5f"; };


  $T<$N T0, $N... TT> log_t< T0 > operator,( log_t< T0 >, TT... ) = delete;
  
  $T<$N T0>
  auto operator,( log_t< T0 > log, endl_t endl ) { 

    log.print( endl );

    return log; 
  }

  $T<$N T0>
  auto operator,( log_t< T0 > log, cstr const& data ) { 

    log.print( log_format< cstr >::format, data );

    return log;
  }

  $T<$N T0, $N T1, char = log_format< T1 >::format[0] >
  auto operator,( log_t< T0 > log, T1 const& data ) { 
    
    log.print( log_format< T1 >::format, data ); 

    return log;
  }

  $T<$N T0, $N T1>
  auto operator,( log_t< T0 > log, lib::value< T1 > const& data ) { 

    log.print( "%s", data->to_string().data() ); 

    return log;
  }

}


