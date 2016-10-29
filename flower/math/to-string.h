#pragma once

#include "lib/macros.h"
#include "lib/types.h"

#include "types.h"

namespace lib {


  TP< TN T0, ssize_t... NN>
  struct string_format< lib::math::vec_t< T0, NN... > > {

    static cstr format( lib::math::vec_t< T0, NN... > const& v ) {

      auto& buffer = global::get_buffer< char >();
      char* ptr = buffer;

      for( auto i : range{ 0, v.size() - 1 } )
        ptr += snprintf( ptr, $array_size( buffer ), "%+.4f, ", v[ i ] );

      snprintf( ptr, $array_size( buffer ), "%+.4f", v[ v.size() - 1 ] );

      return buffer; 
    }

  };

  TP< TN T0, ssize_t... NN>
  struct string_format< lib::math::mat_t< T0, NN... > > {

    static cstr format( lib::math::mat_t< T0, NN... > const& m ) {

      auto& buffer = global::get_buffer< char >();

      char* ptr = buffer;

      for( auto& v : m.data() ) {

        for( auto i : range{ 0, v.size() - 1 } )
          ptr += snprintf( ptr, $array_size( buffer ), "%+.4f, ", v[ i ] );

        ptr += snprintf( ptr, $array_size( buffer ), "%+.4f\n", v[ v.size() - 1 ] );
      }

      *(ptr-1) = '\0';

      return buffer; 
    }

  };


}


