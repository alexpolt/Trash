
#include <cstdio>
#include <vector>

#include "lib/common.h"
#include "lib/dispatch.h"

struct mazda_i : lib::object {
  $interface( mazda_i );
  virtual void set_model( unsigned ) = 0;
  virtual void operator()() = 0;
  virtual ~mazda_i() { }
};

$T<$N T0> 
struct mazda : mazda_i {

  $component( mazda );

  void set_model( unsigned model ) override {
    _owner._model = model;
  }

  void operator()() override {
    printf("%s\n",__func__);
  }

  ~mazda() {
    printf("%s\n",__func__);
  }


};

struct bmw_i : lib::object {
  $interface( bmw_i );
  virtual void set_model( unsigned ) = 0;
};

$T<$N T0> 
struct bmw : bmw_i {

  $component( bmw );

  void set_model( unsigned model ) override {
    _owner._model = model;
  }


};

struct info_i : lib::object {
  $interface( info_i );
  virtual void print() const = 0;
};


$T<$N T0> 
struct info : info_i {

  $component( info );

  void print() const override {
    printf("Hello car!\n");
  }


};

struct car : lib::object {

  car() { }

  ~car() {
    printf("%s\n", to_string().data());
  }

  car( unsigned m ) : _model{ m } { }

  $interface( car );

  $object( car, bmw, mazda  );

  global::buffer to_string() const override {
    global::buffer b;
    snprintf( b.data(), b.size(), "%s%d", "I am car #",  _model );
    return b;
  }

  unsigned _model{};

};


struct print {

  void operator()( value<bmw_i> a, value<info_i> b ) {
    log, a, endl, b, endl;
    printf("dispatch!\n");
  }

  void operator()( value<info_i> a, value<bmw_i> b ) {
    log, a, endl, b, endl;
    printf("dispatch!\n");
  }

  $T<$N U0, $N U1> 
  void operator()( U0 a, U1 b ) {
    log, a, endl, b, endl;
    printf("no dispatch\n");
  }

  using type_return = void;

};

int main() {

  try {

    $on_return {
      printf( "scope destruct\n" );
    };

    void* test[4];
    log, "test[4] = ", $length( test ), ", sizeof = ", (int)sizeof( test ), endl;

    auto car0 = car::create( 1u );

    car0->get_object( mazda_i::tag )();

    printf("bmw = %d\n", car0->get_object( bmw_i::tag )->get_interface_id());

    auto car1 = car::create( 2u );

    printf("mazda = %d\n", car1->get_object( mazda_i::tag )->get_interface_id());

    lib::dispatch< print, bmw_i, info_i, mazda_i > dispatch0;

    dispatch0( car0->get_object( bmw_i::interface_id ), car0->get_object( info_i::interface_id ) );

  } catch( lib::error const& e ) {

    printf( "%s\n", e.what() );

  }

}






