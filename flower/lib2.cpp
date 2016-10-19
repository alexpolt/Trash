
#include "lib/common.h"


struct human : object {

  $interface( human );

  virtual void say_hello() const = 0;
  virtual cstr get_string() const = 0;
};

struct alex : human {

  $object( alex );

  alex() : data{ new string{"Hello World"} } {  }

  ~alex() { info, "Alex: Bye-Bye!", endl; }

  cstr get_string() const override {

    return "I am Alex";
  }

  void say_hello() const override {

    info, "Alex: ", data->data(), endl;
  }

  owner_ptr< string > data;
};


int main() {

  log::memory.on();

  info, "Start Programm", endl;

  auto alex0 = owner< human >::create< alex >();

  auto o0 = alex0->get_object();

  auto human0 = o0->get_object( human::tag );

  auto alex1 = move( alex0 );

  info, "object: ", o0->to_string(), ", human: ", human0->get_string(), endl;


}


