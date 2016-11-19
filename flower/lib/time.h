#pragma once

#include "macros.h"
#include "types.h"
#include "to-string.h"


namespace lib {


  struct time {


    constexpr time( double time = 0 ) : _time{ time } { }

    auto seconds() const { return int( _time ); }

    auto mseconds() const { return int( 1000.f * ( _time - int( _time ) ) ); }

    auto get() const { return _time; }

    float operator()() const { return _time; }

    cstr to_string() const { return lib::to_string( "%.3f", _time ); }

    time operator+( time const& r ) const { return time{ _time + r._time }; }

    bool operator==( time const& r ) const { return _time == r._time; }

    bool operator<( time const& r ) const { return _time < r._time; }

    bool operator!=( time const& r ) const { return not ( $this == r ); }
    bool operator>( time const& r ) const { return r < $this or r == $this; }
    bool operator>=( time const& r ) const { return r < $this; }
    bool operator<=( time const& r ) const { return r > $this; }

    double _time{};
  };


  constexpr auto operator ""_t( long double t ) {

    return time( t );
  }



}


