#pragma once

#include <cstring>
#include <cstdint>

#include "macros.h"
#include "types.h"
#include "murmur-hash.h"


namespace lib {

  TP<TN T>
  struct hash32 {

    using value_type = uint32_t;

    static auto get_hash( void const* value, ssize_t size, int seed = 0 ) {

      return MurmurHash2( value, size, seed );
    }

    TP<TN U, TN = enable_if_t< is_primitive_v< U > >>
    static auto get_hash( U value, int seed = 0 ) {

      return MurmurHash2( &value, $size( value ), seed );
    }

    TP<TN U, TN = enable_if_t< is_container_v< U > >>
    static auto get_hash( U const& value, int seed = 0 ) {

      return MurmurHash2( value.data(), value.size(), seed );
    }

    static auto get_hash( char* value, int seed = 0 ) {

      return MurmurHash2( value, strlen( value ), seed );
    }

    static auto get_hash( cstr value, int seed = 0 ) {

      return MurmurHash2( value, strlen( value ), seed );
    }

  };

  TP<TN T>
  struct hash64 {

    using value_type = uint64_t;

    static auto get_hash( void const* value, ssize_t size, int seed = 0 ) {

      return MurmurHash64A( value, size, seed );
    }

    TP<TN U, TN = enable_if_t< is_primitive_v< U > >>
    static auto get_hash( U value, int seed = 0 ) {

      return MurmurHash64A( &value, $size( value ), seed );
    }

    TP<TN U, TN = enable_if_t< is_container_v< U > >>
    static auto get_hash( U const& value, int seed = 0 ) {

      return MurmurHash64A( value.data(), value.size(), seed );
    }

    static auto get_hash( char* value, int seed = 0 ) {

      return MurmurHash64A( value, strlen( value ), seed );
    }

    static auto get_hash( cstr value, int seed = 0 ) {

      return MurmurHash64A( value, strlen( value ), seed );
    }

  };

}

