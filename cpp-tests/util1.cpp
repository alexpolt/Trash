#include <cstdio>
#include <cassert>

struct A {};
struct B : A {};
struct C : B {};
struct D {};
struct E : private A {};

  template<class A, class B> struct is_base_of { 
    static char isbaseof( A* );
    static int isbaseof( ... );
    static const int value = sizeof(  isbaseof( (B*)0 )  ) == 1;
  };

int main() {


printf( "is_base_of<A, A>::value == 1\t%u\n", is_base_of<A, A>::value == 1 );
printf( "is_base_of<B, A>::value == 1\t%u\n", is_base_of<B, A>::value == 1 );
printf( "is_base_of<A, B>::value == 1\t%u\n", is_base_of<A, B>::value == 1 );
printf( "is_base_of<A, C>::value == 1\t%u\n", is_base_of<A, C>::value == 1 );
printf( "is_base_of<C, A>::value == 1\t%u\n", is_base_of<C, A>::value == 1 );
printf( "is_base_of<A, D>::value == 1\t%u\n", is_base_of<A, D>::value == 1 );
printf( "is_base_of<D, B>::value == 1\t%u\n", is_base_of<D, B>::value == 1 );
//printf( "is_base_of<A, E>::value == 1\t%u\n", is_base_of<A, E>::value == 1 );

  

return 0;
}


