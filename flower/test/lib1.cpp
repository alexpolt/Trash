
#include "../lib/common.h"


int main() {

  lib::log::memory.on();

  info, "Start Programm", endl;

  string s0{ '*' };
  string s1;

  s1 = s0;

  info, s1, endl;
  info, s0, endl;

  vector<int> v0;
  
  range{ 0, 10 } $do { v0 << s1; };

  info, v0, endl;


}

