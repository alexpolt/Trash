#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"


namespace lib {

  namespace render {

    
    struct resource {

      enum type {

        buffer, shader, texture
      };

      virtual ~resource() { }

      virtual type get_type() const = 0;
    };


    TP<TN T>
    T* resource_cast( resource* base ) { 
      
      $assert( base->get_type() == T::type, "resource cast failed" );

      return ( T* ) base;
    }


    struct message {

      enum type : int {

        clear, mesh, material, cbuffer, shader, size
      };

      static cstr get_desc( type type ) {

        static cstr type_desc[]{ "command", "mesh", "material", "cbuffer", "shader" };

        return type_desc[ type ];
      }

      virtual ~message() { }
    };


    //struct colorf 

  }
}




