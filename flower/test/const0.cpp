#include <type_traits>
#include "../lib/common.h"

int main() {

  int const a = 5;
  auto& b = a;

  info, std::is_const< lib::no_const_t<lib::no_ref_t<decltype(b)>> >::value, ", ", lib::is_const_v< decltype(b) >, endl;
  info, std::is_reference< lib::no_ref_t<decltype(b)> >::value, ", ", lib::is_ref_v< decltype(b) >, endl;

}

