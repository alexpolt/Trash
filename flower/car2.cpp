
#include <cstdio>
#include <vector>

#include "lib/common.h"
#include "lib/dispatch.h"

struct car : object {
  $interface( car );
  using model_t = unsigned;
  virtual void set_model( model_t ) = 0;
};


struct car_info : object {
  $interface( car_info );
  virtual void print() const = 0;
};


$t<$n T0> 
struct bmw_info : car_info {

  $component( bmw_info );

  void print() const override {
    printf("Hello car!\n");
  }

  lib::string to_string() const override {
    return "object info_basic : info";
  }

  T0& object;

};

$t<$t<$n> class ... TT> 
struct bmw : car {

  $object( bmw );

  bmw( model_t m ) : model{ m } { printf( "create %d\n", m ); }

  lib::string to_string() const override {
    return lib::string{"I am car#"} + std::to_string( model );
  }

  void set_model( model_t m ) override {
    model = m;
  }

  model_t model{};

};

$t<$t<$n> class ... TT> 
struct lada : bmw< TT... > {
  using bmw< TT... >::bmw;
};

struct print {

  void operator()( value<car> a, value<car> b ) {
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

    auto car0 = bmw< bmw_info >::create( 1u );
    value<object> lada0 = lada<>{ 2u };

    printf("bmw = %d, size = %d\n", car0->get_object_id(), sizeof( car0 ) );

    auto car0_info = car0->get_object( car_info::tag );

    car0_info->print();

    printf( "car_info0 size = %d, lada0 to_string: %s\n", sizeof( car0_info ), lada0->to_string().data() );

    value<car> car1 = car0_info->get_object( car::tag );

    printf( "value<car> size = %d, to_string: %s\n", sizeof(car1), car1->to_string().data() );

    lib::dispatch< print, car > dispatch0;
    dispatch0( car0, lada0 );

  } catch( lib::error const& e ) {

    printf( "%s\n", e.what() );

  }

}






