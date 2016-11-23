#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/to-string.h"
#include "lib/ptr.h"

namespace lib {

  namespace render {

    struct resource;

    using resource_ptr = strong_ptr< resource >;
    
    struct resource {

      enum class rtype : uint {

        null, buffer, shader, texture,
      };

      static cstr get_type_desc( rtype type ) {
        static cstr desc[]{ "null", "buffer", "shader", "texture" };
        return desc[ (int) type ];
      }

      enum class rformat : uint { 

        null, float3, uint3, uint1,
      };

      static cstr get_format_desc( rformat fmt ) {
        static cstr desc[]{ "null", "vertex", "normal", "color" };
        return desc[ (int) fmt ];
      }


      virtual ~resource() { }

      virtual rtype type() const = 0;
      virtual rformat format() const = 0;
      virtual void* data() const = 0;
      virtual ssize_t size() const = 0;
      virtual ssize_t value_size() const = 0;

      virtual cstr to_string() const {

        return lib::to_string( 
          "%s( format = %s, size = %d )", get_type_desc( type() ), get_format_desc( format() ), size() );
      }

   };


    TP<TN T>
    T* resource_cast( resource* base ) { 
      
      $assert( base->type() == T::type, "resource cast failed" );

      return ( T* ) base;
    }


    struct message {

      enum mtype : int {

        null, clear, mesh, material, cbuffer, shader, size
      };

      static cstr get_desc( mtype type ) {

        static cstr type_desc[]{ "command", "mesh", "material", "cbuffer", "shader" };

        return type_desc[ type ];
      }

      cstr to_string() const { return "uknown render operation"; }

      virtual ~message() { }
    };



  }
}




