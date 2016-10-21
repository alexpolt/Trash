

#include "lib/log.h"

struct test1 {
  char const* to_string() const { return "Helllllo"; }
};

struct test2 {

};



int main() {


  using log::info;
  using log::endl;

  info, "test:", test1{}, ", test2: ", [](){ return "AAAAA!";}, "123", 123, (void*)0, endl;

}

