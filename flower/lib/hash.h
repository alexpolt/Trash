#pragma once

#include <cstdint>

#include "macros.h"
#include "types.h"
#include "murmur-hash.h"
#include "ptr.h"


namespace lib {

  struct has_to_hash {
    using yes = char(&)[1];
    using no = char(&)[2];
    TP<TN T, decltype( &T::to_hash ) = &T::to_hash> static yes check( int );
    TP<TN> static no check( ... );
  };

  TP<TN T = void>
  struct hash32 {

    using value_type = uint32_t;

    static auto get_hash( void const* value, ssize_t size, int seed = 0 ) {

      return MurmurHash2( value, size, seed );
    }

    TP<TN U, TN = enable_if_t< is_primitive_v< U > >>
    static auto get_hash( U value, int seed = 0 ) {

      return MurmurHash2( &value, $size( value ), seed );
    }

    TP<TN U, TN = enable_if_t< is_container_v< U > and $size( has_to_hash::check< U >( 0 ) ) == 2 >>
    static auto get_hash( U const& value, int seed = 0 ) {

      return MurmurHash2( value.data(), value.size(), seed );
    }

    TP<TN U, TN = enable_if_t< $size( has_to_hash::check< U >( 0 ) ) == 1 >, TN = void>
    static auto get_hash( U const& value, int seed = 0 ) {

      return value.to_hash( seed );
    }


    static auto get_hash( char* value, int seed = 0 ) {

      return MurmurHash2( value, strlen( value ), seed );
    }

    static auto get_hash( cstr value, int seed = 0 ) {

      return MurmurHash2( value, strlen( value ), seed );
    }

  };


  TP<> struct hash32< object_sp > {

    using value_type = oid_t;

    TP<TN T>
    static auto get_hash( T const& o, int seed = 0 ) {

      return o->get_object_id() + seed;
    }
  };


  TP<> struct hash32< object_wp > {

    using value_type = oid_t;

    TP<TN T>
    static auto get_hash( T const& o, int seed = 0 ) {

      return o->get_object_id() + seed;
    }
  };


  TP<TN T = void>
  struct hash64 {

    using value_type = uint64_t;

    static auto get_hash( void const* value, ssize_t size, int seed = 0 ) {

      return MurmurHash64A( value, size, seed );
    }

    TP<TN U, TN = enable_if_t< is_primitive_v< U > >>
    static auto get_hash( U value, int seed = 0 ) {

      return MurmurHash64A( &value, $size( value ), seed );
    }

    TP<TN U, TN = enable_if_t< is_container_v< U > and $size( has_to_hash::check< U >( 0 ) ) == 2 >>
    static auto get_hash( U const& value, int seed = 0 ) {

      return MurmurHash64A( value.data(), value.size(), seed );
    }

    TP<TN U, TN = enable_if_t< $size( has_to_hash::check< U >( 0 ) ) == 1 >, TN = void>
    static auto get_hash( U const& value, int seed = 0 ) {

      return value.to_hash( seed );
    }

    static auto get_hash( char* value, int seed = 0 ) {

      return MurmurHash64A( value, strlen( value ), seed );
    }

    static auto get_hash( cstr value, int seed = 0 ) {

      return MurmurHash64A( value, strlen( value ), seed );
    }

  };

}

