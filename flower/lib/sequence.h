#pragma once

#include "atomic.h"


namespace lib {

  struct atomic_sequence {

    oid_t next() { return _source.add( 1 ); }

    lib::atomic< oid_t > _source{};

  };

  TP<TN T0 = oid_t>
  atomic_sequence id_source{};

  namespace global {

    inline oid_t gen_object_id() { return id_source<>.next(); }

    TP<TN T0>
    inline oid_t gen_id() { return id_source< T0 >.next(); }

  }

}


