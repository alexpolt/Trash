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

      mesh() { log::info, "mesh()", log::endl; }

      ~mesh() { log::info, "~mesh()", log::endl; }
    };


  }
}


