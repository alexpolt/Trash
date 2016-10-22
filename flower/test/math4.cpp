
#include "../lib/common.h"
#include "../math/common.h"


int main () {


  //libm::vec3f v0{ $cos( 10_rad ), $sin( 10_rad ), 1 };

  libm::vec3f v0{ 1, 0, 1 };

  info, "v0 = ", v0, endl;

  info, "rotz = \n", libm::rotz< float, 10 >, endl;

  range{ 0, 10 } $do {

    v0 = libm::rotz< float, 10 > * v0;

    info, "rotz( v0 ) = ", v0, endl;
  };
}


