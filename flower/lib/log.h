#pragma once

#include <cstdio>
#include "macros.h"
#include "types.h"
#include "value.h"

namespace log {


  TP<TN T0> 
  struct log_t : T0 { };
  
  struct endl_t { };

  struct ignore_t {
    TP<TN... TT>
    static void print( TT... args ) { }
  };

  struct printer_t {
    TP<TN... TT>
    static void print( TT... args ) { printf( args... ); }
    static void print( endl_t ) { printf( "\n" ); }
  };

  log_t< printer_t > info;
  log_t< printer_t > error;
  log_t< ignore_t > debug;
  log_t< ignore_t > memory;
  endl_t endl;


  TP<TN T0> struct log_format { };

  TP<> struct log_format< char > { constexpr static cstr format = "%c"; };
  TP<> struct log_format< cstr > { constexpr static cstr format = "%s"; };
  TP<> struct log_format< char* > { constexpr static cstr format = "%s"; };
  TP<> struct log_format< bool > { constexpr static cstr format = "%d"; };
  TP<> struct log_format< uint > { constexpr static cstr format = "%d"; };
  TP<> struct log_format< int > { constexpr static cstr format = "%d"; };
  TP<> struct log_format< void* > { constexpr static cstr format = "%p"; };
  TP<> struct log_format< float > { constexpr static cstr format = "%.5f"; };
  TP<> struct log_format< double > { constexpr static cstr format = "%.5f"; };


  TP<TN T0, TN... TT> log_t< T0 > operator,( log_t< T0 >, TT... ) = delete;
  
  TP<TN T0, TN T1, TN = lib::enable_if_t< lib::is_container< T1 >::value >>
  auto operator,( log_t< T0 > log, T1 const& data ) { 

    for( auto& e : data ) log, e, ", ";

    return log; 
  }

  TP<TN T0>
  auto operator,( log_t< T0 > log, endl_t endl ) { 

    log.print( endl );

    return log; 
  }

  TP<TN T0>
  auto operator,( log_t< T0 > log, cstr const& data ) { 

    log.print( log_format< cstr >::format, data );

    return log;
  }

  TP<TN T0>
  auto operator,( log_t< T0 > log, char* data ) { 

    log.print( log_format< char* >::format, data );

    return log;
  }

  TP<TN T0, TN T1>
  auto operator,( log_t< T0 > log, T1* data ) { 

    log.print( log_format< void* >::format, (void*) data );

    return log;
  }

  TP<TN T0, TN T1, char = log_format< T1 >::format[0] >
  auto operator,( log_t< T0 > log, T1 const& data ) { 
    
    log.print( log_format< T1 >::format, data ); 

    return log;
  }

  TP<TN T0, TN T1>
  auto operator,( log_t< T0 > log, lib::value< T1 > const& data ) { 

    log.print( "%s", data->to_string().data() ); 

    return log;
  }

}


