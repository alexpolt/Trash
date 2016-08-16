#include <cstdio>
#include "polynom.h"

int main() {

int even{}, odd{};

auto prn =  [&](const char* str, int koef = -1, int pos = -1 ) {
  if( pos != -1 ) {
    if( koef % 2 == 0 ) even++;
    else odd++;
  } else {
    //printf( "\n\n" );
  }
};

auto p1 = Polynom<int>{1,1,1,1,1,  1,1,1,1,1,  1,1,1};
auto p2 = Polynom<int>{1,1,1,1,1,  1,1,1,1,1,  1,1};
auto p3 = Polynom<int>{1,1,1,1,1};

auto r1 = p1 * p1;
r1.print( prn );
printf("even = %d, odd = %d\n", even, odd );
even  = odd = 0;

auto r2 = p2 * p2;
r2.print( prn );
printf("even = %d, odd = %d\n", even, odd );
even  = odd = 0;


auto r3 = p3 * p3;
r3.print( prn );
printf("even = %d, odd = %d\n", even, odd );
even  = odd = 0;


return 0;
}
