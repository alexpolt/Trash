#pragma once

#include "macros.h"
#include "types.h"
#include "oid-generator.h"
#include "vector.h"


namespace global {

  using namespace lib;

  $T<$N T0, int N0 = 0>
  struct objects {

    thread_local static T0 array[ N0 ];

    thread_local static oid_generator_t oid_source;

  };

  $T<$N T0, int N0>
  thread_local T0 objects< T0, N0 >::array[ N0 ];

  $T<$N T0, int N0>
  thread_local oid_generator_t objects< T0, N0 >::oid_source;


  oid_t get_object_id() { return objects< oid_t >::oid_source.next(); }


  $T<$N T0 = char, lib::size_t N0 = 1024 >
  vector< T0, N0 > buffer{ objects< T0, N0 >::array };

}


