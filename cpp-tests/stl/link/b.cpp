#include "a.h"

//template<> void test<char>() {}

//template struct Test<char>;

void b() {
  test<A<char>>();
}
