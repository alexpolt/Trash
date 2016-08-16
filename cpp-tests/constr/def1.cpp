#include <cstdio>

struct A {
 	A() {}
	A(int i) : m(i) {}
	int m;
};
struct B : virtual A {
	B() {}
	B(int i):A(i),m(i) {}
	int m;
};
struct C : virtual A {
	C() {}
	C(int i) : A(i), m(i) {}
	int m;
};
struct D : B, C {
	D() {}
	D(int i) : B(i), C(i) {}
	int m;
};



int main() {

D b(1);
printf("%d\n", b.A::m );
printf("%d\n", b.B::m );

return 0;
}
