#include <cstdio>
#include <memory>

struct B {
	int a;
	~B(){}
	virtual void test(){}
};
struct Test {
	int a;
	B b;
	//Test() { printf("There was a call\n"); }
	~Test() {}
	virtual void test(){}
};


int main () {
	char buf[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	for(auto i : buf ) printf( "%d ", i );
	printf( "\n" );
	new (buf) Test();
	for(auto i : buf ) printf( "%d ", i );
	printf( "\n" );
}
