
#include "lib/common.h"

#include "math/common.h"

int main() {

  lib::log::memory.on();

  lib::m::vec3f v0{ 1.f, 1.f, 1.f };
  lib::m::vec3f v1{ 2.f, 2.f, 2.f };

  info, "v0 = ", v0, endl, endl;
/*

  info, "v0 = "; for( auto i : v0.data() ) info, i, ", "; info, endl;
  info, "v1 = "; for( auto i : v1.data() ) info, i, ", "; info, endl;


  info, endl;

  {
    auto v2 = v0+v1;
    info, "v0 + v1 = "; for( auto i : v2.data() ) info, i, ", "; info, endl;
  }

  {
    auto v2 = v0-v1;
    info, "v0 - v1 = "; for( auto i : v2.data() ) info, i, ", "; info, endl;
  }

  {
    auto v2 = v0*v1;
    info, "v0 * v1 = "; for( auto i : v2.data() ) info, i, ", "; info, endl;
  }

  {
    auto v2 = v0/v1;
    info, "v0 / v1 = "; for( auto i : v2.data() ) info, i, ", "; info, endl;
  }

  {
    auto v2 = dot( v0, v1 );
    info, "v0 . v1 = "; info, v2, endl;
  }

  info, endl;

  {
    auto v2 = normalize( v0 );
    info, "normalize( v0 ) = "; for( auto i : v2.data() ) info, i, ", "; info, endl;
  }

*/



}

