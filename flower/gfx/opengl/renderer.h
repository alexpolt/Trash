#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "render/queue.h"
#include "gl/gl.h"


namespace lib {

  namespace gfx {

  namespace gl {


    void renderer() {

      if( render::available() ) {

        log::gfx, "process render queue", log::endl;
      }
    }


  }
  }
}


