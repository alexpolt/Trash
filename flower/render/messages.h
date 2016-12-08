#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"
#include "message.h"
#include "color.h"
#include "buffer.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"


namespace lib {

  namespace render {

  namespace messages {


    struct clear : message::base {
      
      static constexpr msg::type type = msg::type::clear;

      cstr to_string() const { 

        return lib::to_string( 
          "clear( r = %u, g = %u, b = %u, a = %u, depth = %.5f )", 
          color.r(), color.g(), color.b(), color.a(), depth ); 
      }

      color color{};
      float depth{ -1 };
    };


    struct vbuffer : message::base {
      
      static constexpr msg::type type = msg::type::vbuffer;

      cstr to_string() const { return vbuffer->to_string(); }

      vbuffer_ptr vbuffer;
    };



  }
  }
}



