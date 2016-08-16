#include <cstdio>
#include <cassert>
#include <random>
#include <functional>
#include <chrono>

#include "../config.h"
#include "../testing.h"
#include "../heap.h"
#include "../usertypes/range.h"

using namespace alexpolt;


int main() {

TEST_BEGIN( "heap" );
//TEST(  );

auto seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine engine{ seed };
std::uniform_int_distribution<int> distr{1, 1000};
auto dice = std::bind( distr, engine );

size_t c = dice();
size_t* data = heap::malloc<size_t>( c );

for( size_t off: range<>( 0, c ) ) data[ off ] = dice();
for( size_t off: range<>( 0, c ) ) printf( "%zu ", data[ off ] );
printf("\n");

heap::print();

range<>* rng = heap::malloc< range<> >( );

new( rng ) range<>{ 1, 10 };

heap::print();

printf("rng->end_m = %d\n", rng->end_m);

heap::free( rng );

heap::print();

TEST_END();

return 0;
}


