
#include <stdio.h>

double n0 = 0.5;
double n1 = -0.5;
double n2 = 0.49999999999999994;
double n3 = -0.49999999999999994;

int round_naive( double n ) {
    
  return n + 0.5;
}

int round_less_naive( double n ) {
  
  return n > 0.0 ? n + 0.5 : n - 0.5;
}

int round_my_try( double n ){
    
  int n2 = (int)( n * 2.0 ); // double it
  int bit = n2 & 0x1; // extract the half-bit
  unsigned sign = n2 >> 31; // extract sign info
  return  n2/2 + sign ? -bit : bit; // adjust
}

void test( double n ) {
    
  printf( "round_naive      ( %.17f ) -> %d\n", n, round_naive( n ) );
  printf( "round_less_naive ( %.17f ) -> %d\n", n, round_less_naive( n ) );
  printf( "round_my_try     ( %.17f ) -> %d\n\n", n, round_my_try( n ) );
}

int main(void) {
    
  test( n0 );
  test( n1 );
  test( n2 );
  test( n3 );
    
  return 0;
}

