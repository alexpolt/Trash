#include <cstdio>

#define virtual virtual

struct a1 {};
struct a2 { virtual void test() {printf("test a2\n");} };
struct a3 {  };
struct b : a1, a3, a2 { virtual void test() {printf("test b\n");} };



int main(){

b b;

printf("b(%d)=%p a1(%d)=%p a2(%d)=%p a3(%d)=%p\n", sizeof (b), &b,  sizeof (a1), (a1*)&b, sizeof (a2), (a2*)&b, sizeof (a3), (a3*)&b );
b.test();

return 0;
}

