#pragma once

#include "atomic.h"


namespace lib {

  struct atomic_sequence {

    oid_t next() { return _source.add( 1 ); }

    lib::atomic< oid_t > _source{};

  };

  TP<TN T0 = oid_t>
  auto& get_oid_source() { 

    static atomic_sequence oid_source{};

    return oid_source;
  }

  oid_t gen_object_id() { return get_oid_source().next(); }

}


