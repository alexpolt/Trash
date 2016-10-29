
#include <cstdio>

#include "lib/common.h"
#include "math/common.h"


int main() {

  lib::log::memory.on();
  lib::log::lock.on();
  lib::log::task.on();

  info, "start program", endl;

  auto c = $cos( math::radians( 20 ) );
  auto s = $sin( math::radians( 20 ) );
  auto c2 = $cos( math::pi / 3. );
  auto s2 = $sin( math::pi / 3. );

  int skipped = 0;

  for( auto z : range{ -3, 4 } ) 
  for( auto a : range{ -5, 5 } ) {

  auto zs = sin( math::radians(1.5*z) ), zc = cos( math::radians(1.5*z) );
  auto sa = sin( math::radians(1.5*a) ), ca = cos( math::radians(1.5*a) );

  auto v = $normalize( vec4d{ zc*ca, zc*sa, zs, 0 } );

  auto m = mat4d {

    vec4d{  v[0], -v[1],  v[2], -v[3] },
    vec4d{  v[1],  v[0], -v[3], -v[2] },
    vec4d{  v[2],  v[3],  v[0],  v[1] },
    vec4d{ -v[3],  v[2],  v[1], -v[0] },
  };

  range{ 0, 100 } $do {

    auto v1 = vec4d{ 100 } * v;
    v = m * v;
    auto v2 = vec4d{ 100 } * v;
    if(length(v)>100) { skipped++;  }
    else {
      info, "v ", v1[ 0 ], " ", v1[ 1 ], " ", v1[ 2 ], " ", endl;
      info, "v ", v1[ 0 ], " ", v1[ 1 ] + 2, " ", v1[ 2 ], " ", endl;
      info, "v ", v2[ 0 ], " ", v2[ 1 ], " ", v2[ 2 ], " ", endl;
    }
    //info, length( v ), endl;

  };
  }

  //info, endl;
  //return 0;

  for( auto i : range{ 0, 7000-skipped } ) {

    info, "f ", i*3+1, " ", i*3+2, " ", i*3+3, endl;

  };



}


