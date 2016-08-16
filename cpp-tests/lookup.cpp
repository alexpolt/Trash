#include <cstdio>

struct myint { operator int(){return 10;} };

int foo(int x) { return 1; }

template<class T>
int f(T a) { return foo(a+x); }

int foo(long x) { return 2; }

// POI
void f2() { printf("%d\n", f<myint>(myint{}) ); } // uses foo(int)

int x;

int main() {
	f2();
}

