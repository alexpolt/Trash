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

$T<$N T0> struct car_ai_basic;

struct lada : car {

  $object( lada, car_ai_basic );

  void update() override {
    $clobber();
    ++*_angle;
  }

  uint get_counter() override {
    return *_angle;
  }

  lib::string to_string() const override {
    return "lada";
  }

  uint* _angle = new uint{};
};

struct mazda : lada {
   void update() override {
     $clobber();
    *_angle+=1;
  }
};


$T<$N T0> struct car_ai_basic : car_ai {

  $component( car_ai_basic );

  void steer( dir_t dir ) override {
    if( dir == left ) 
      --_owner._angle;
    else
      ++_owner._angle;
  }

  uint get_steer() const override {
    return *_owner._angle;
  }

};



$T<$N T0>
void measure( T0& v );

int main() {

  for( auto i : range{ 0, 5 } ) log, i, " ";

  log, endl;

  try {

    const int car_size = 512;

    value< car > car0 = value<car>::create< lada >();
    log, "log: ", car0, endl;
    printf("1\n");
    auto car_ai0 = car0->get_object( car_ai::tag );
    printf("1\n");
    //auto car_copy0 = car0->get_copy();
    printf("1\n");
    auto car_object0 = car0->get_object();

    printf("car %s steer %s\n", car0->to_string().data(), car0->get_counter() == car_ai::left ? "left":"right");

    car_ai0->steer( car_ai::right );

    printf("car %s steer %s\n", car_ai0->to_string().data(), car0->get_counter() == car_ai::left ? "left":"right");

    printf("value<car> size = %d\n", $size( car0 ) );

    std::vector< value<car> > v0{ car_size, car0 };
    std::vector<car*> v1( car_size );

    $escape( &v0 );
    $escape( &v1 );

    v0[ 0 ] = value<car>::create< mazda >();
    v1[ 0 ] = new mazda{};

    for( auto i : range{ 0, car_size } ) {
      v0[ i ] = value<car>::create< lada >();
      v1[ i ] = new lada{};
    }

    measure( v0 );
    measure( v1 );

  } catch( lib::error& e ) {

    printf( "%s\n", e.what() );

  }

}

$T<$N T0>
void measure( T0& v0 ) {

  auto begin = std::chrono::high_resolution_clock::now();


  for( uint i{}; i < 100000; i++ )
    for( auto& v : v0 ) 
      v->update();


  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  printf( "car1 = %d %d %d, dt = %ld\n", v0[0]->get_counter(), v0[1]->get_counter(), v0[2]->get_counter(), (long)dt );

}



