#include <cstdio>
#include <vector>
#include <chrono>

#include "lib/common.h"


struct car : object {

  $interface( car );

  void virtual update( int ) = 0; 

  uint virtual get_counter() = 0;

  //static const uint type_size = 8;

};

struct car_ai : object {

  $interface( car_ai );

  enum dir_t { left, right };

  virtual void steer( dir_t ) = 0;
  virtual uint get_steer( ) const = 0;

};

TP<TN T0> struct car_ai_basic;

struct lada : car {

  $object( lada, car_ai_basic< lada > );

  void update( int ) override {
    $clobber();
    ++*_angle;
  }

  uint get_counter() override {
    return *_angle;
  }

  cstr to_string() const override {
    return "lada";
  }

  //~lada(){ info, "~lada( ", this, " )", endl; }

  uint* _angle = new uint{};
};

struct mazda : lada {
   void update( int ) override {
     $clobber();
    *_angle+=1;
  }
  //~mazda(){ info, "~mazda( ", this, " )", endl; }
};


TP<TN T0> struct car_ai_basic : car_ai {

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

  ~car_ai_basic(){ info, "~car_ai_basic()", endl; }
};



void measure0();
void measure1();

int main() {

  try {

    const int car_size = 256;

    auto c0 = owner< car >::create< lada >();
    c0 = owner< car >::create< lada >();

    owner<car> car0 = owner< car >::create< mazda >();

    info, "log: ", c0, endl;

    auto car_ai0 = car0->get_object( car_ai::tag );
    auto car_copy0 = car0->get_copy();
    auto car_object0 = car0->get_object();

    printf("car %s steer %s\n", car0->to_string(), car0->get_counter() == car_ai::left ? "left":"right");

    car_ai0->steer( car_ai::right );

    printf("car %s steer %s\n", car_ai0->to_string(), car0->get_counter() == car_ai::left ? "left":"right");

    printf("value<car> size = %d\n", $size( car0 ) );

    log::memory.on();

    info, endl, "-- measure0 --", endl;
    measure0();

    info, endl, "-- measure1 --", endl;
    measure1();


  } catch( lib::error& e ) {

    printf( "%s\n", e.what() );

  }

}

void measure0() {

  vector< owner< car > > v;

  auto begin = std::chrono::high_resolution_clock::now();


  for( auto i : range{ 0, 5'000 } ) {

    v << owner< car >::create< mazda >();

    for( auto i : range{ 0, 1024 } ) v << owner< car >::create< lada >();

    $escape( &v );

    for( auto& e : v ) e->update( i );

    $clobber();

    v.clear();
  }


  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  printf( "dt = %ld\n", (long)dt );

}

void measure1() {

  vector< lib::owner_ptr<car> > v;

  auto begin = std::chrono::high_resolution_clock::now();


  for( auto i : range{ 0, 5'000 } ) {

    v << lib::make_owner< mazda >();

    for( auto i : range{ 0, 1024 } ) v << lib::make_owner< lada >(); 

    $escape( &v );

    for( auto& e : v ) e->update( i );    

    $clobber();

    v.clear();
  }


  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  printf( "dt = %ld\n", (long)dt );

}


