#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "os/types.h"
#include "opengl-funcs.h"


namespace lib {

  namespace gfx {


    struct opengl_win32 : nocopy {


      opengl_win32( os::hwnd hwnd ) {

        log::gfx, "create opengl context", log::endl;
      }

      ~opengl_win32() {

        log::gfx, "destory opengl context", log::endl;
      }

    };


  }
}


