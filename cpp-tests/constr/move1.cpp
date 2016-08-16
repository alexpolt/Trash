#include <cstdio>

struct A {
	A() = default;
	A(A&& a) {
		printf("A move\n");
	}
	A(const A& a) {
		printf("A copy\n");
	}
	void test() {}
};
struct B : A {
	B() = default;
};

int main() {

B a; 
B b ( a );
B b2( static_cast<B&&>( b ) );

return 0;
}

