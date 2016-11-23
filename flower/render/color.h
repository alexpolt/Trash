#pragma once

#include "lib/macros.h"
#include "lib/types.h"


namespace lib {

  namespace render {


    struct color {

      static constexpr uint shift_r = 0;
      static constexpr uint shift_g = 8;
      static constexpr uint shift_b = 16;
      static constexpr uint shift_a = 24;

      static constexpr uint mask = 0xFF;
      static constexpr uint mask_r = mask << shift_r;
      static constexpr uint mask_g = mask << shift_g;
      static constexpr uint mask_b = mask << shift_b;
      static constexpr uint mask_a = mask << shift_a;

      color() { }

      color( int r, int g, int b, int a ) {

        set_r( r );
        set_g( g );
        set_b( b );
        set_a( a );
      }

      auto r() const { return ( _data & mask_r ) >> shift_r; }
      auto g() const { return ( _data & mask_g ) >> shift_g; }
      auto b() const { return ( _data & mask_b ) >> shift_b; }
      auto a() const { return ( _data & mask_a ) >> shift_a; }

      auto rf() const { return float( r() ) / mask; }
      auto gf() const { return float( g() ) / mask; }
      auto bf() const { return float( b() ) / mask; }
      auto af() const { return float( a() ) / mask; }

      void set_r( uint value ) { _data = ( _data & ~mask_r ) | ( ( value & mask ) << shift_r ); }
      void set_g( uint value ) { _data = ( _data & ~mask_g ) | ( ( value & mask ) << shift_g ); }
      void set_b( uint value ) { _data = ( _data & ~mask_b ) | ( ( value & mask ) << shift_b ); }
      void set_a( uint value ) { _data = ( _data & ~mask_a ) | ( ( value & mask ) << shift_a ); }

      auto data() const { return _data; }

      uint _data;
    };

  }
}


