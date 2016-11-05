#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"

namespace lib {


  TP<TN T, bool is_weak> 
  struct strong_ptr;

  struct object;

  using object_sp = strong_ptr< object, false >;

  using object_wp = strong_ptr< object, true >;


}


