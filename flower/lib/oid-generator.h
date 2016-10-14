#pragma once

#include <atomic>


namespace lib {

  struct oid_generator_t {

    oid_t next() { return _source.fetch_add( 1 ); }

    std::atomic< oid_t > _source{};

  };

}


