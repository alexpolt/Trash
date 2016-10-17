
#include "log.h"
#include "sequence.h"

namespace lib {


  struct test {
    
    test() { oid = gen_object_id(); log::info, "test_", oid, "_()", log::endl; }

    test( test const& o ) { log::info, "test_", oid, "_(& ", o.oid, ")", log::endl; }

    test( test&& o ) { log::info, "test_", oid, "_(&& ", o.oid, ")", log::endl; }

    test& operator=( test const& o ) { log::info, "operator_", oid, "_(& ", o.oid, ")", log::endl; return $this; }

    test& operator=( test&& o ) { log::info, "operator_", oid, "_(&& ", o.oid, ")", log::endl; return $this; }

    ~test() { log::info, "~test_", oid, "_()", log::endl; }

    oid_t oid;

  };


  auto make_test() { return new test{}; } 

}


