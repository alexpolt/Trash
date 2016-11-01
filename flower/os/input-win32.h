#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "lib/macros.h"
#include "lib/types.h"
#include "keymap.h"


namespace lib {
  namespace os {


    struct input {

      void operator()() {

        PeekMessage();
      }

    }


  }
}


