
#include <cstdio>

#include "lib/common.h"
#include "math/common.h"


int main() {

  lib::log::memory.on();
  lib::log::lock.on();
  lib::log::task.on();

  info, "start program", endl;

  auto c = $cos( math::pi / 40. );
  auto s = $sin( math::pi / 40. );
  auto c2 = $cos( math::pi / 3. );
  auto s2 = $sin( math::pi / 3. );



  vec4d v = $normalize( vec4d{ c*c,  0, c*s,  s } );

  auto m = mat4d {

    vec4d{  v[0], -v[1],  v[2], -v[3] },
    vec4d{  v[1],  v[0], -v[3], -v[2] },
    vec4d{ -v[2],  v[3],  v[0], -v[1] },
    vec4d{  v[3],  v[2],  v[1],  v[0] },
  };

  for( auto a : range{ 0, 20 } ) {

  auto sa = sin( math::radians(18.*a) ), ca = cos( math::radians(18.*a) );

  v = $normalize( vec4d{ ca, sa, 0, 0 } );

  range{ 0, 25 } $do {

    auto v1 = vec4d{ 100 } * v;
    v = v * m;
    auto v2 = vec4d{ 100 } * v;

    info, "v ", v1[ 0 ], " ", v1[ 1 ], " ", v1[ 2 ], " ", endl;
    info, "v ", v1[ 0 ], " ", v1[ 1 ] + 2, " ", v1[ 2 ], " ", endl;
    info, "v ", v2[ 0 ], " ", v2[ 1 ], " ", v2[ 2 ], " ", endl;

    //info, length( v ), endl;

  };
  }

  info, endl;
  //return 0;

  for( auto i : range{ 0, 500 } ) {

    info, "f ", i*3+1, " ", i*3+2, " ", i*3+3, endl;

  };



}


