
#include "lib/common.h"

void string3( string s ) {

  lib::log::memory.on();

  info, "string3(),", s.data(), endl,"int: ", 1,2,1.5,endl;

  lib::log::memory.off();
}

