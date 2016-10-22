#pragma once

#include "../lib/macros.h"
#include "../lib/types.h"
#include "../lib/error.h"

#include "types.h"

namespace lib {


  TP< TN T0, ssize_t... NN>
  struct string_format< libm::vec_t< T0, NN... > > {

    static cstr format( libm::vec_t< T0, NN... > const& v ) {

      auto& buffer = global::get_buffer< char >();
      char* ptr = buffer;

      for( auto i : range{ 0, v.size() - 1 } )
        ptr += snprintf( ptr, $length( buffer ), "%.5f, ", v[ i ] );

      snprintf( ptr, $length( buffer ), "%.5f", v[ v.size() - 1 ] );

      return buffer; 
    }

  };

  TP< TN T0, ssize_t... NN>
  struct string_format< libm::mat_t< T0, NN... > > {

    static cstr format( libm::mat_t< T0, NN... > const& m ) {

      auto& buffer = global::get_buffer< char >();

      char* ptr = buffer;

      for( auto& v : m.data() ) {

        for( auto i : range{ 0, v.size() - 1 } )
          ptr += snprintf( ptr, $length( buffer ), "%.5f, ", v[ i ] );

        ptr += snprintf( ptr, $length( buffer ), "%.5f\n", v[ v.size() - 1 ] );
      }

      *(ptr-1) = '\0';

      return buffer; 
    }

  };


}


