
#include "lib/common.h"
#include "math/common.h"

using namespace libm;

constexpr veci< 2 > fibo( ssize_t i, veci< 2 > v0 ) {

  if( i == 15 ) return v0;

  mati< 2 > m0{ veci< 2 >{ 1, 1 }, veci< 2 >{ 1, 0 } };

  v0 = m0 * v0;
    
  return fibo( ++i, v0 );
}

TP<ssize_t N0> struct print_result { 

  static_assert( N0 == 0, "result" );
};

int main() {


  print_result< fibo( 0, veci< 2 >{1, 0} )[0] >{};


}


