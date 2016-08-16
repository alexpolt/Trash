struct Test1 {

template<class A, class B = decltype( A::test() )> static auto test(int) -> decltype( A::test() );
//template<class A, class B = typename A::test, class C = void > static double test(int);
//template<class A> static char test(unsigned int);

};
struct Test2 {
	static void test2();
	//int test;
	//using test = void;
	//struct test {};
	
};

static_assert( sizeof( Test1::test<Test2>(0) ) == 1, "no" );