#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/vector.h"


namespace lib {

  namespace render {


    TP<TN T>
    struct buffer {

      using value_type = T;
      using vector = vector< T >;
      static constexpr ssize_t value_size = $size( value_type );

      enum format_t { null, vertex, normal, color };

      buffer() { }

      buffer( format_t fmt, vector_f data = vector_f{} ) : _format{ fmt }, _data{ data } { }

      auto format() { return _format; }
      auto& data() { return _data; }
      auto size() { return value_size; }

      void set_format( format_t fmt ) { _format = fmt; }
      void set_data( vector_b data ) { _data = move( data ); }

      
      format_t _format;
      vector _data;
    };


  }
}




