
#include "lib/log.h"
#include "lib/types.h"
#include "lib/ptr.h"
#include "lib/test.h"

using namespace lib;
using namespace log;


auto get_test() { return owner_ptr< test >{ make_test() }; }

struct a { };

int main() {


  owner_ptr< void > o0{ new test{} };

  owner_ptr< a > o1{ new a{} };

  o0 = move( o1 );

  info, "---", endl;



}


