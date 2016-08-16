#include <cstdio>
#include <typeinfo>

template<class T>
void test( T& t ) {
	printf("%s\n", typeid( t ).name() );
	printf("%d\n", sizeof(T) );
}

int main() {
int a[]={1,2,3};
test( a );

return 0;
}
