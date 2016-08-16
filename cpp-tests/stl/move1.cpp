
#include <cstdio>

namespace Test {

template<int N> struct tagIndex {}; // helper
template<class N> struct tagType {};  // helper

template<int S, class... T> struct Tuple_;  // forward

template<class... T> struct Tuple : Tuple_<sizeof...(T), T...> {

	Tuple( T... args ) : Tuple_<sizeof...(T), T...> ( args... ) {}

	template<int N> auto get() -> decltype(  this->get1( tagIndex<N>() )  )  { 

			return this->get1( tagIndex<N>() ); 
	}

	template<class U> U get() {  return this->get2( tagType<U>() );  }

};

template<int S, class H, class... T> struct Tuple_<S, H, T...> : Tuple_<S, T...> { // main class, S - number of arguments
	H value;
	enum { index = Tuple_<S, T...>::index - 1 };

	Tuple_( H h, T... args ) : Tuple_<S, T...> ( args... ), value(h) {}

	H get1( tagIndex<index> ) { return value; }
	H get2( tagType<H> ) { return value; }

	using Tuple_<S, T...>::get1; // very useful for name lookup
	using Tuple_<S, T...>::get2; // all the way to the base class
};

template<int S> struct Tuple_<S> { 
	void get1();
	void get2();
	enum { index = S }; 
};

}

using Tuple1 = Test::Tuple<int, char, float, int>;

volatile void test ( Tuple1* st ) {
	printf( "%d\n", st->get<0>()  ); // 10
	printf( "%.2f\n", st->get<2>()  ); // 3.00
	printf( "%d\n", st->get<int>()  ); // 10, first int
}

int main() {

Tuple1 st{ 10, 2, 3, 5 };

test( &st );

return 0;
}
