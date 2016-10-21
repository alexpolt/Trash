
#include "../lib/log.h"
#include "../lib/types.h"
#include "../lib/ptr.h"


struct test1 {};

struct test2 {
  static const bool is_primitive = true;
};


int main() {

  //auto& info = log::info;
  //auto& endl = log::endl;
  //using log::operator,;

  info, lib::is_primitive_v<int const>, endl;
  info, lib::is_primitive_v<int*>, endl;
  info, lib::is_primitive_v<int* const>, endl;
  info, lib::is_primitive_v<int const*>, endl;
  info, lib::is_primitive_v<char*>, endl;
  info, lib::is_primitive_v<char&>, endl;
  info, lib::is_primitive_v<test1>, endl;
  info, lib::is_primitive_v<test2>, endl;


}


