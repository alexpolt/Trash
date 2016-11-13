#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/ptr.h"
#include "linker.h"


namespace lib {

  namespace global {

    TP<TN T = object_wp>
    linker< T > link_map{};
  }

}


