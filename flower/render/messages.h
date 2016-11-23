#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"
#include "types.h"
#include "color.h"
#include "buffer.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"


namespace lib {

  namespace render {

  namespace messages {


    struct clear : message {
      
      static constexpr mtype type = mtype::clear;

      cstr to_string() const { 

        return lib::to_string( 
          "clear( r = %u, g = %u, b = %u, a = %u, depth = %.5f )", 
          color.r(), color.g(), color.b(), color.a(), depth ); 
      }

      color color{};
      float depth{ -1 };
    };

    struct mesh : message {
      
      static constexpr mtype type = mtype::mesh;

    };

    struct cbuffer : message {
      
      static constexpr mtype type = mtype::cbuffer;

    };

    struct shader : message {
      
      static constexpr mtype type = mtype::shader;

    };


  }
  
  }
}



