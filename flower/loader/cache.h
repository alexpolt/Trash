#pragma once

#include "lib/macros.h"
#include "lib/types.h""
#include "lib/vector.h"
#include "lib/hash-map.h"
#include "lib/url.h"
#include "lib/alloc-default.h"


namespace lib {

  namespace loader {

  namespace global {


    TP<TN T>
    hash_map< url, T > cache{ alloc_default::create( "loader cache" ) };

  }
  }
}


