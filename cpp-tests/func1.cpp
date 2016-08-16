#include <cstdio>

void test1( void(&func)() ) { printf("test1\n"); }

void test2() {}

int main() {

  test1( test2 );
}
