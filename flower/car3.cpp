#include <cstdio>
#include <vector>
#include <chrono>

#include "lib/common.h"


struct car : object {

  $interface( car );

  void virtual operator()() = 0;

  uint virtual get_counter() = 0;

  //static const uint type_size = 256;

};


struct lada : car {

  $object( lada );

  void operator()() override {
    counter++;
  }

  uint get_counter() override {
    return counter;
  }
  
  uint counter{};
};

$t<$n T0>
void measure( T0& v );

int main() {

  value< car > car0 = value<car>::create< lada >();

  printf("value<car> size = %d\n", sizeof( car0 ) );

  std::vector< value<car> > v0{ 256, car0 };

  measure( v0 );

  std::vector<car*> v1;

  for( uint i{}; i < 256; i++ ) v1.push_back( new lada{} );

  measure( v1 );

}

$t<$n T0>
void measure( T0& v0 ) {

  auto begin = std::chrono::high_resolution_clock::now();


  for( uint i{}; i < 1000000; i++ )

    for( auto& v : v0 ) v->operator()();


  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  printf( "car = %d, dt = %ld\n", v0[0]->get_counter(), (long)dt );
}



