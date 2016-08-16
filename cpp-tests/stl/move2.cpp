#include <cstdio>
#include <type_traits>
#include <typeinfo>


#define CRITICAL_SIZE 16

template<class T> struct RefOrVal { using type = typename std::conditional< std::is_reference<T>::value, T, 
					typename std::conditional< (sizeof(T)>16), T, T& >::type >::type; };

template<class T>
void push( const typename RefOrVal<T&&>::type value ) {
	printf("%s\n", typeid(RefOrVal<T>::type).name() );
}

struct A {
char data[16];
	A() {}
	A(A&&) {
		printf("moving!\n");
	}
};
struct B{
	B() {}
	B(B&&) {
		printf("moving!\n");
	}
};

int main() {
A a;
push ( a );

return 0;
}
