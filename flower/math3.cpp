
#include "lib/common.h"
#include "math/common.h"

using namespace libm;

TP<ssize_t angle>
constexpr vec3f rot( ssize_t i, vec3f v0 ) {

  if( i == 0 ) return v0;

  v0 = rotz< angle > * v0;

  return rot< angle >( --i, v0 );
}

TP<ssize_t N0> struct print_result { 

  static_assert( N0 == 0, "result" );
};

int main() {


  print_result< ssize_t( rot< 36 >( 5, vec3f{ 1.f, 0.f, 0.f } )[0] ) >{};


}


