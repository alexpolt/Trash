
#include <cstdio>
#include <vector>

#include "../lib/common.h"
#include "../lib/dispatch.h"

struct car : object {
  $interface( car );
  using model_t = unsigned;
  virtual void set_model( model_t ) = 0;
};


struct car_info : object {
  $interface( car_info );
  virtual void print() const = 0;
};


$T<$N T0> 
struct bmw_info : car_info {

  $component( bmw_info );

  void print() const override {
    printf("Hello car!\n");
  }

  global::buffer to_string() const override {
    return "object info_basic : info";
  }


};

$T<$T<$N> class ... TT> 
struct bmw : car {

  $object( bmw, bmw_info );

  bmw( model_t m ) : _model{ m } { printf( "create %d\n", m ); }

  global::buffer to_string() const override {
    global::buffer b;
    snprintf( b.data(), b.size(), "%s%d", "I am car #",  _model );
    return b;
  }

  void set_model( model_t m ) override {
    _model = m;
  }

  model_t _model{};

};

$T<$T<$N> class ... TT> 
struct lada : bmw< TT... > {
  using bmw< TT... >::bmw;
};

struct print {

  void operator()( value<car> a, value<car> b ) {
    printf("dispatch!\n");
  }

  $T<$N U0, $N U1> 
  void operator()( U0 a, U1 b ) {
    printf("no dispatch\n");
  }

  using type_return = void;

};

int main() {

  try {

    auto car0 = bmw< bmw_info >::create( 1u );
    value<object> lada0 = lada<>{ 2u }.get_object();

    printf("bmw = %d, size = %d\n", car0->get_interface_id(), $size( car0 ) );

    auto car0_info = car0->get_object( car_info::tag );

    car0_info->print();

    printf( "car_info0 size = %d, lada0 to_string: %s\n", $size( car0_info ), lada0->to_string().data() );

    value<car> car1 = car0_info->get_object( car::tag );

    printf( "value<car> size = %d, to_string: %s\n", $size(car1), car1->to_string().data() );

    lib::dispatch< print, car > dispatch0;
    dispatch0( car0, lada0 );

  } catch( lib::error const& e ) {

    printf( "%s\n", e.what() );

  }

}






