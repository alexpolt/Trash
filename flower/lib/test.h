
#include "log.h"
#include "sequence.h"


namespace lib {


  struct test {
    
    test() { _oid = global::gen_id< test >(); log::info, "test_", _oid, "_()", log::endl; }

    test( test const& o ) { log::info, "test_", _oid, "_(& ", o._oid, ")", log::endl; }

    test( test&& o ) { log::info, "test_", _oid, "_(&& ", o._oid, ")", log::endl; }

    test& operator=( test const& o ) { log::info, "operator_", _oid, "_(& ", o._oid, ")", log::endl; return $this; }

    test& operator=( test&& o ) { log::info, "operator_", _oid, "_(&& ", o._oid, ")", log::endl; return $this; }

    ~test() { log::info, "~test_", _oid, "_()", log::endl; }

    void print() { log::info, "test::print( ", _oid, " )", log::endl; }

    oid_t _oid;

  };


  inline auto make_test() { return new test{}; } 

}


