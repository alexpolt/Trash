
#include "lib/ptr.h"
#include "lib/log.h"

struct test {
  test(int v) : _v{v} { 
    //lib::log, "test(",(int)this,")\n";
  }
  ~test() {
    //lib::log, "~test(",(int)this,")\n";
  }
  int _v;
};

lib::own_ptr<test> test2() {
  return lib::own_ptr<test>{ new test{3} };
}

int main() {

  lib::own_ptr<test> o0{ new test{1} }, o1{ (lib::own_ptr<test>&&)o0 };


  lib::own_ptr<test> o2; o2 = test2();

  lib::own_ptr<int[]> o3;


  lib::log, o1->_v, lib::endl;

}


