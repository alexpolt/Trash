#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/vector.h"
#include "lib/hash-map.h"
#include "lib/url.h"
#include "lib/alloc-default.h"
#include "render/resource.h"


namespace lib {

  namespace loader {

  namespace global {


    TP<TN...>
    hash_map< url, render::resource_ptr > cache{ 16, alloc_default::create( "loader cache" ) };

  }
  }
}


