#include "a.h"

//template<> void test<char>() {}
//extern void b();

template<class T> struct Test<A<T>> {
	void test();
};
template<class T> void Test<A<T>>::test() {};

int main() {

test<A<char>>();


return 0;
}
