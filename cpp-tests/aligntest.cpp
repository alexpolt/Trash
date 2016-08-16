#include <windows.h>
#include <stdio.h>


struct test {
  WORD a;
  DWORD b;
};

int main () {

printf("align test: %u, size = %u\n", alignof( test ), sizeof (test) );

return 0;
}
