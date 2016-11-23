#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/vector.h"
#include "lib/alloc-default.h"
#include "lib/ptr.h"
#include "types.h"


namespace lib {

  namespace render {


    TP<TN T> 
    struct buffer;

    TP<TN T>
    using buffer_ptr = strong_ptr< buffer< T > >;

    using ibuffer = buffer< uint >;
    using vbuffer = buffer< float >;
    using cbuffer = buffer< uint >;

    using ibuffer_ptr = buffer_ptr< uint >;
    using vbuffer_ptr = buffer_ptr< float >;
    using cbuffer_ptr = buffer_ptr< uint >;


    TP<TN T>
    struct buffer : resource {

      using value_type = T;
      using vector_type = vector< T >;

      static constexpr rtype _type = resource::rtype::buffer;
      static constexpr ssize_t _value_size = $size( value_type );

      static auto create_alloc() { return alloc_default::create( "resource buffer" ); }


      buffer() { }

      buffer( rformat fmt, vector_type data = vector_type{ create_alloc() } ) : _format{ fmt }, _data{ data } { }


      rtype type() const override { return _type; }
      rformat format() const override { return _format; }
      void* data() const override { return _data.data(); }
      ssize_t size() const override { return _data.size_bytes(); }
      ssize_t value_size() const { return _value_size; }
      
      auto& vector() const { return _data; }
      
      cstr to_string() const override {

        return lib::to_string( "buffer( format = %s, size = %d )", get_format_desc( format() ), size() );
      }

      void set_format( rformat fmt ) { _format = fmt; }
      void set_data( vector_type data ) { _data = move( data ); }

      rformat _format{};
      vector_type _data{ create_alloc() };
    };


  }
}




