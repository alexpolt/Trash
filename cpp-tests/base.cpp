#include <cstdio>
#include <typeinfo>

template<class T> struct tag {};

template<class T> struct base { void print() { printf( "class %s\n", typeid(T).name()); } };

struct A1 {};
struct A2 {};
struct A3 {};

template<class... T> struct test : base<T>... {
    template<class A> void print(tag<A>) { base<A>::print(); };
};

int main() {


test<A1,A2,A3> t;

t.print( tag<A1>() );
t.print( tag<A2>() );
t.print( tag<A3>() );

printf("sizeof test %d\n", sizeof t);

}
