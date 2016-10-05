#include <cstdio>
#include <vector>
#include <chrono>

#include "lib/common.h"


struct car : object {

  $interface( car );

  void virtual update() = 0; 

  uint virtual get_counter() = 0;

  //static const uint type_size = 8;

};

struct car_ai : object {

  $interface( car_ai );

  enum dir_t { left, right };

  virtual void steer( dir_t ) = 0;
  virtual uint get_steer( ) const = 0;

};

$t<$n T0> struct car_ai_basic;

struct lada : car {

  $object( lada, car_ai_basic );

  void update() override {
    $clobber();
    angle++;
  }

  uint get_counter() override {
    return angle;
  }
  
  int angle{};
};

struct mazda : lada {
   void update() override {
     $clobber();
    angle+=2;
  }
};


$t<$n T0> struct car_ai_basic : car_ai {

  $component( car_ai_basic, lada );

  void steer( dir_t dir ) override {
    if( dir == left ) 
      object.angle--;
    else
      object.angle++;
  }

  uint get_steer() const override {
    return object.angle;
  }

  lada& object;
};


$t<$n T0>
void measure( T0& v );

int main() {

  const int car_size = 512;

  value< car > car0 = value<car>::create< lada >();
  auto car_ai0 = car0->get_object( car_ai::tag );

  printf("car %s steer %s\n", car0->to_string().data(), car0->get_counter() == car_ai::left ? "left":"right");

  car_ai0->steer( car_ai::right );

  printf("car %s steer %s\n", car_ai0->to_string().data(), car0->get_counter() == car_ai::left ? "left":"right");

  printf("value<car> size = %d\n", sizeof( car0 ) );

  std::vector< value<car> > v0{ car_size, car0 };
  std::vector<car*> v1( car_size );

  $escape( &v0 );
  $escape( &v1 );

  v0[ 1 ] = value<car>::create< mazda >();

  for( uint i{}; i < car_size; i++ ) v1[ i ] = new lada{};

  v1[ 1 ] = new mazda{};

  measure( v0 );
  measure( v1 );

}

$t<$n T0>
void measure( T0& v0 ) {

  auto begin = std::chrono::high_resolution_clock::now();


  for( uint i{}; i < 1000000; i++ )
    for( auto& v : v0 ) 
      v->update();


  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  printf( "car1 = %d, dt = %ld\n", v0[0]->get_counter(), (long)dt );
}



