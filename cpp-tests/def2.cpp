#include <cstdio>

#define $str(str) #str

const char str[] = $str( alex );

int main() {

printf( $str( alex ) );

}
