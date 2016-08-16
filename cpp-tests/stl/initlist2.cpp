#include <initializer_list>
#include <typeinfo>
#include <cstdio>
#include <tuple>
#include <vector>

struct Test {
	template< class T>
	Test( std::initializer_list<T> list );
};

template< class T>
Test::Test( std::initializer_list<T> list )  {  printf( "Type: %s\n", typeid( *(list.begin()) ).name() ); }

int main() {

Test t1{10.0,5.0,3.0};
Test t2{1,2,3};
Test t3{"a","b"};
Test t4{"a"};


return 0;
}
