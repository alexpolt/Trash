#pragma once

#include "common.h"

namespace lib {

  struct log_t { } log;
  struct endl_t { } endl;

  $T<$N T0> struct log_format { };

  $T<> struct log_format< cstr > : type_true { constexpr static cstr format = "%s"; };
  $T<> struct log_format< int > : type_true { constexpr static cstr format = "%d"; };


  $T<$N T0, $N T1> void operator,( T0, T1 ) = delete;

  log_t operator,( log_t log, endl_t ) { printf( "\n" ); return log; }

  log_t operator,( log_t log, cstr const& data ) { 
    printf( log_format< cstr >::format, data );
    return log;
  }

  $T<$N T0, bool = log_format< T0 >::value >
  log_t operator,( log_t log, T0 const& data ) { 
    printf( log_format< T0 >::format, data ); 
    return log;
  }

  $T<$N T0>
  log_t operator,( log_t log, value< T0 > const& data ) { 
    printf( "%s", data->to_string().data() ); 
    return log;
  }

}


