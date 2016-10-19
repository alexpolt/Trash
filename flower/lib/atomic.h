#pragma once

#include "macros.h"
#include "types.h"

#ifndef $CONFIG_NO_ATOMIC

  #include <atomic>

#endif

namespace lib {

  #ifndef $CONFIG_NO_ATOMIC

    TP< TN U0 >
    struct atomic {

      U0 add( U0 value, std::memory_order order = std::memory_order_relaxed ) { return _data.fetch_add( value, order ); }
      U0 sub( U0 value, std::memory_order order = std::memory_order_relaxed ) { return _data.fetch_sub( value, order ); }

      void store( U0 value, std::memory_order order = std::memory_order_relaxed ) { _data.store( value, order ); }
      U0 load( std::memory_order order = std::memory_order_relaxed ) const { return _data.load( order ); }

      U0 exchange( U0 desired, std::memory_order order = std::memory_order_relaxed ) { return _data.exchange( desired, order ); }

      bool compare_exchange_weak( U0& expected, U0 desired, std::memory_order order = std::memory_order_relaxed ) {
        return _data.compare_exchange_weak( expected, desired, order );
      }

      void operator=( U0 value ) { store( value ); }
      operator U0() { return load(); }

      std::atomic< U0 > _data;
    };

  #else

    TP< TN U0 >
    struct atomic {

      U0 add( U0 value ) { auto tmp = _data; _data+=value; return tmp; }
      U0 sub( U0 value ) { auto tmp = _data; _data-=value; return tmp; }

      void store( U0 value ) { _data = value; }

      U0 load( ) const { return _data; }

      U0 exchange( U0 desired ) { return swap( _data, desired ); }

      bool compare_exchange_weak( U0& expected, U0 desired ) {

        if( _data == expected ) {

          swap( _data, desired );

          return true;

        } else {

          expected = _data;

          return false;
        }
      }

      auto& operator=( U0 value ) { _data = value; return $this; }

      operator U0() { return _data; }

      U0 _data;
    };

  #endif

}


