
#include "../lib/common.h"



int main(){

  liblog::memory.on();
  
  info, "Start", endl;

  string s;

  s = "1234 ";  s << ':';  s << (void*)&s;  s<< " * ";
  s << 41.5; s << " * "; s << 60u; s << " * ";
  s << -13;

  info, endl, s.data(), endl, endl;
  info, s.size(), ", ", s, endl;

}

