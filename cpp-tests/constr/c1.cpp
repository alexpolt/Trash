#include <cstdio>

struct Test {
	struct true_{}; struct false_{};
	template<class T, class... Args> 
	struct HasCreate {
		template<class U, T (*p)(Args...) = T::create> static true_ yes( int );
		template<class U> static false_ yes( char );
		using value = decltype( yes<T>( 0 ) );
	};

	template<class T, class A> struct enable_if;
	template<class A> struct enable_if<false_, A>;
	template<class A> struct enable_if<true_, A> { using value = A; };

	template<class T, class...> struct Not;
	template<class... T> struct Not<true_,T...> { using value = false_; };
	template<class... T> struct Not<false_,T...> { using value = true_; };

	template<class T, class... ARGS > static typename enable_if< typename HasCreate<T, ARGS...>::value, T >::value create( ARGS... args ) {
		printf("Create1\n");
		return T::create( args... );
	}
	template<class T, class... ARGS > static typename enable_if< typename Not<typename HasCreate<T, ARGS...>::value>::value, T >::value create( ARGS... args ) {
		printf("Create2\n");
		return T( args... );
	}
};

struct Test2 : Test {
	static Test2 create(int i) {
		printf("Create3\n");
		return Test2(i);
	}
	Test2(int){}
};

struct Test3 : Test {};

int main() {

auto t1 = Test::create<Test2>(1);
auto t2 = Test::create<Test3>();

}
