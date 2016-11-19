#pragma once

#include "windows.h"
#include "lib/time.h"


namespace lib {

  namespace os {


    void sleep( time t ) {

      Sleep( t.mseconds() );
    }

  }
}

