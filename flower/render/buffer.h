#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/vector.h"
#include "lib/alloc-default.h"
#include "lib/ptr.h"
#include "resource.h"


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
    struct buffer : res::base {

      using value_type = T;
      using vector_type = vector< T >;

      static constexpr res::type _type = res::type::buffer; 
      static constexpr ssize_t _value_size = $size( value_type );

      static auto create_alloc() { return alloc_default::create( "resource buffer" ); }


      buffer() { }

      buffer( url url, res::format fmt, vector_type data ) : 
        _url{ url }, _format{ fmt }, _data{ move( data ) } { }

      res::type type() const override { return _type; }
      res::format format() const override { return _format; }
      void* data() const override { return _data.data(); }
      ssize_t size() const override { return _data.size_bytes(); }
      ssize_t value_size() const override { return _value_size; }
      url url() const override { return _url; }

      auto& vector() const { return _data; }
      
      cstr to_string() const override {

        return lib::to_string( "buffer( format = %s, size = %d )", get_format_desc( format() ), size() );
      }

      void set_format( res::format fmt ) { _format = fmt; }
      void set_data( vector_type data ) { _data = move( data ); }

      url _url;
      res::format _format{};
      vector_type _data{ create_alloc() };
    };


  }
}




