#pragma once

#include "macros.h"
#include "types.h"
#include "to-string.h"


namespace lib {


  struct time {

    constexpr time() { }

    constexpr time( uint s, uint ms = 0 ) {

      _s = s + ms / 1000;

      _ms = ms % 1000;
    }


    uint seconds() const { return _s; }

    uint mseconds() const { return _ms; }

    float operator()() const { return float( _s ) + float( _ms ) / 1000.f; }

    cstr to_string() const { return lib::to_string( "%u.%0.3u", _s, _ms ); }


    time operator+( time const& r ) const { 

      return time{ _s + r._s, _ms + r._ms }; 
    }

    bool operator==( time const& r ) const { 

      return _s == r._s and _ms == r._ms; 
    }

    bool operator<( time const& r ) const { 

      return _s < r._s or ( _s == r._s and _ms < r._ms );
    }

    bool operator!=( time const& r ) const { return not ( $this == r ); }
    bool operator>( time const& r ) const { return r < $this or r == $this; }
    bool operator>=( time const& r ) const { return r < $this; }
    bool operator<=( time const& r ) const { return r > $this; }


    uint _s{};
    uint _ms{};
  };


  constexpr auto operator ""_s( unsigned long long seconds ) {

    return time( seconds );
  }

  constexpr auto operator ""_ms( unsigned long long mseconds ) {

    return time( 0, mseconds );
  }



}


