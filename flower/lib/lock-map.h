#pragma once

#include "macros.h"
#include "types.h"

#include "locker.h"


namespace lib {

  namespace global {

    TP<TN T = void>
    locker lock_map{};

    inline void trigger_lock_map() { (void)lock_map<>; }
  }

}



