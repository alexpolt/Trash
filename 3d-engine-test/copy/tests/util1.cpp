#include <cstdio>
#include <cassert>
#include "../util.h"
#include "../testing.h"


struct A {};
struct B : A {};
struct C : B {};
struct D {};

using namespace alexpolt;

int main() {

TEST_BEGIN( "util" );

TEST( ( util::is_base_of<A, B>::value == 1 ) );
TEST( ( util::is_base_of<B, A>::value == 0 ) );
TEST( ( util::is_base_of<A, C>::value == 1 ) );
TEST( ( util::is_base_of<C, A>::value == 0 ) );
TEST( ( util::is_base_of<A, D>::value == 0 ) );
TEST( ( util::is_base_of<D, B>::value == 0 ) );
  
TEST_END();

return 0;
}


