
#include <cstdio>

extern "C" int __stdcall GetStdHandle(unsigned);

extern "C" int __stdcall WinMain( int, unsigned, char*, int ) {
  int out = GetStdHandle( -11 );
  if( out == 0 ) fopen("win1.null", "w+b");
  else if ( out == -1 ) fopen("win1.invl", "w+b");
  else fopen("win1.yes", "wb");

  return 0;
}
  