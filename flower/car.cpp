
#include <cstdio>
#include <vector>

#include "lib/common.h"
#include "lib/dispatch.h"

struct mazda_i : lib::object {
  $interface( mazda_i );
  virtual void set_model( unsigned ) = 0;
};

$t<$n T0> 
struct mazda : mazda_i {

  $component( mazda );

  void set_model( unsigned model ) override {
    object.model = model;
  }

  T0& object;

};

struct bmw_i : lib::object {
  $interface( bmw_i );
  virtual void set_model( unsigned ) = 0;
};

$t<$n T0> 
struct bmw : bmw_i {

  $component( bmw );

  void set_model( unsigned model ) override {
    object.model = model;
  }

  T0& object;

};

struct info_i : lib::object {
  $interface( info_i );
  virtual void print() const = 0;
};


$t<$n T0> 
struct info : info_i {

  $component( info );

  void print() const override {
    printf("Hello car!\n");
  }

  T0& object;

};

struct car : lib::object {

  car() { }

  car( unsigned m ) : model{ m } { }

  $interface( car );
  $object( car, bmw, mazda, info  );

  lib::string to_string() const override {
    return lib::string{"I am car#"} + std::to_string( model );
  }

  unsigned model{};

};


struct print {

  void operator()( value<bmw_i> a, value<info_i> b ) {
    printf("dispatch!\n");
  }

  $t<$n U0, $n U1> 
  void operator()( U0 a, U1 b ) {
    printf("no dispatch\n");
  }

  using type_return = void;

};

int main() {

  try {

    auto car0 = car::create( 1u );
    printf("bmw = %d\n", car0->get_object( bmw_i::tag )->get_interface_id());

    auto car1 = car::create( 2u );
    printf("mazda = %d\n", car1->get_object( mazda_i::tag )->get_interface_id());

    lib::dispatch< print, bmw_i, info_i, mazda_i > dispatch0;

    dispatch0( car0->get_object( bmw_i::interface_id ), car0->get_object( info_i::interface_id ) );

  } catch( lib::error const& e ) {

    printf( "%s\n", e.what() );

  }

}






