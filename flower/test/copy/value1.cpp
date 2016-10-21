
#include "lib/types.h"
#include "lib/value.h"
#include "lib/log.h"

struct test {
  int c;
};

int main(){

  int a = 5;

  test t = lib::type_cast< test >( a );

  log::info, "a = ", a, ", test.c = ", t.c, log::endl;

  auto v0 = lib::value< int >::create<int>( 2 );

  log::info, "sizeof value<int> = ", $size( v0 ), ", v0 = ", *v0, log::endl;

}

