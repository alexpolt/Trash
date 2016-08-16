#define $stringify(arg) #arg
#define $concat_(arg1,arg2) arg1 ## arg2
#define $concat(arg1,arg2) $concat_(arg1,arg2)
#define $rawstr(arg) $concat( R, $stringify((arg)) )
#define $str(arg) arg


#include <cstdio>
#include <unistd.h>



int main() {
  auto& str = $str( R"(hellow world baby i am here no \n yes!)" );
  printf("sizeof %d\n", sizeof str);
  write( STDOUT_FILENO, str, sizeof str);
}

