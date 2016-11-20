#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "types.h"
#include "buffer.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"


namespace lib {

  namespace render {

  namespace messages {


    struct clear : message {
      
      static constexpr message::type type = message::type::clear;

      //color c;
      //depth d;
    };

    struct mesh : message {
      
      static constexpr message::type type = message::type::mesh;

    };

    struct cbuffer : message {
      
      static constexpr message::type type = message::type::cbuffer;

    };

    struct shader : message {
      
      static constexpr message::type type = message::type::shader;

    };


  }
  }
}


