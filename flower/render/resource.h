#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/to-string.h"
#include "lib/ptr.h"
#include "lib/url.h"


namespace lib {

  namespace render {

  namespace resource {


      enum class type : uint {

        null, buffer, shader, texture, size
      };

      static cstr get_type_desc( type type ) {
        static cstr desc[ (ssize_t) type::size ]{ "null", "buffer", "shader", "texture" };
        return desc[ (int) type ];
      }

      enum class format : uint { 

        null, float3, uint3, uint1, size
      };

      static cstr get_format_desc( format fmt ) {
        static cstr desc[ (ssize_t) format::size ]{ "null", "float3", "uint3", "uint1" };
        return desc[ (int) fmt ];
      }


      struct base : nocopy_vbase {

        virtual type type() const = 0;
        virtual format format() const = 0;
        virtual void* data() const = 0;
        virtual ssize_t size() const = 0;
        virtual ssize_t value_size() const = 0;
        virtual url url() const = 0;

        virtual cstr to_string() const {

          return lib::to_string( 
            "%s( format = %s, size = %d )", get_type_desc( type() ), get_format_desc( format() ), size() );
        }

      };

    }

    namespace res = resource;

    using resource_ptr = strong_ptr< res::base >;

    TP<TN T>
    T* resource_cast( res::base* base ) { 
      
      $assert( base->type() == T::type, "resource cast failed" );

      return ( T* ) base;
    }


  }
}




