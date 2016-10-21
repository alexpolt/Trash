
#include "lib/common.h"


int main() {

  lib::log::memory.on();

  vector< int > v0{ 1, 2, 3, 4 };

  string s0;

  s0 << "test me";

 info, "0: ", s0, endl;

  auto t = lib::make_vector( lib::global::get_buffer< int, 4 >() );

  t<<1<<2;

  info, "5: ", t, endl;
  
  vector< int > v2;
  
  v2 = move( t );

  info, "5: ", t, endl;
  info, "1: ", v2, endl;
  info, "11: ", s0, endl;

  vector< long > v1;

  v1 << v0 << move( v1 );

  info, "1: ", v1, endl;

  info, "1: ", v0, endl;
  info, "1: ", v0, endl;

}

