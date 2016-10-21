
#include <thread>

#include "../lib/common.h"


int main() {

  lib::log::memory.on();

  info, "Hi!", endl;
  
  string s{ "test" };

  info, s, endl;

  vector< int > v{ s };

  info, v, endl;

}

