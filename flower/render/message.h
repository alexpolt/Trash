#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "types.h"
#include "ibuffer.h"
#include "vbuffer.h"
#include "cbuffer.h"
#include "texture.h"
#include "shader.h"


namespace lib {

  namespace render {

        
    struct mesh : message::base {
      
      static constexpr message::type type = message::type::mesh;

      vbuffer vb;
      ibuffer ib;
    };

    struct cbuffer : message::base {
      
      static constexpr message::type type = message::type::cbuffer;

      cbuffer vs;
      cbuffer ps;
    };

    struct shader : message::base {
      
      static constexpr message::type type = message::type::shader;

      shader vs;
      shader ps;
    };



  }
}


