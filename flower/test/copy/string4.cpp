
#include "lib/common.h"



int main(){

  lib::log::memory.on();
  
  info, "Start", endl;

  string s;

  s = "1234 ";  s << ':';  s << &s;  s<< " * ";
  s << 41.5; s << " * "; s << 60u; s << " * ";
  s << -13;

  info, s.size(), ", ", s, endl;

}

