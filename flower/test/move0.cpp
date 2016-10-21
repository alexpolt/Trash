
#include "../lib/common.h"

struct test {};

int main() {

  test t0;

  test t1 = move( t0 );

  int a  = 5;

  int b = move ( a );

  info, a, ", ", b, endl;


};

