#include <iostream>
#include <typeinfo>

struct Test {

template<class T> operator T&&() { 
	std::cout << typeid(T).name() << std::endl; return static_cast<T&&>( T() ); 
}

};

int main() {
Test t;
int a = t;
float b = t;


return 0;
};

