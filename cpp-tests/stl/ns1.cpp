#include <type_traits>

namespace Test {

template<class... T> void typeerror(T...);
template<class... T> void typeerror(T&...);

template<class T> void typerror(T);
template<class T> void typerror(T&);


	void test() {
		int a, b;
		auto a1 = [a]() {};
		auto b1 = [b]() {};
		//printf("%s == $s\n", typeid(a).name(), typeid(b).name());
		
		typeerror( a1, b1 );
		//typeerror( std::is_same<decltype(a1),decltype(2)>::value );
	}

}

