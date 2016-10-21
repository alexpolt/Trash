
#include "lib/common.h"

int main() {

  vector< int > v0;

  for( auto i : range{ 0, 7 } ) v0 << 1;

  info, "v0 = ", v0, endl;

  vector< int > v1;

  v1 = v0;

  info, "v0 = ", v0, endl;
  info, "v1 = ", v1, endl;


}

