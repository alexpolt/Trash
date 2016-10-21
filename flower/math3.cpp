
#include "lib/common.h"
#include "math/common.h"

using namespace libm;

TP<ssize_t angle>
constexpr vec3f rot( ssize_t i, vec3f v0 ) {

  if( i == 0 ) return v0;

  v0 = rotz< angle > * v0;

  return rot< angle >( --i, v0 );
}



constexpr double print( double d ) { return print( d ); }

#define $static_print( $0 ) struct $var { \
  constexpr static double data1 = print( $0 ); \
  constexpr static int data2 = { $0 }; \
};

//constexpr auto cos89 = cos_cexpr( radians< double >( 89 ) );
//constexpr auto cos179 = cos_cexpr( radians< double >( 179 ) );
//constexpr auto cos265 = cos_cexpr( radians< double >( 265 ) );
//constexpr auto cos350 = cos_cexpr( radians< double >( 350 ) );

constexpr auto s1 = sin_cexpr( radians< double >( 1 ) );
//constexpr auto s100 = sin_cexpr( radians< double >( 100 ) );
//constexpr auto s265 = sin_cexpr( radians< double >( 265 ) );
//constexpr auto s710 = sin_cexpr( radians< double >( 710 ) );


//$static_print( cos89 );
//$static_print( coi179 );
//$static_print( cos265 );    
//$static_print( cos350 );

$static_print( s1 );
//$static_print( s100 );
//$static_print( s265 );    
//$static_print( s710 );


//$static_print( sqrt_cexpr( 25 ) );

//$static_print( rot< 36 >( 5, vec3f{ 1.f, 0.f, 0.f } )[0] );



