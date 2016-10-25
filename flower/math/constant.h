#pragma once

#include "../lib/macros.h"
#include "../lib/types.h"


namespace lib {

    namespace math {

    TP<TN T0>
    constexpr T0 pi = T0( 3.14159265358979 );

    TP<TN T0>
    constexpr T0 pi12 = T0( .5 ) * pi< T0 >;

    TP<TN T0>
    constexpr T0 pi34 = pi< T0 > + pi12< T0 >;

    TP<TN T0>
    constexpr T0 pi2 = pi< T0 > + pi< T0 >;

    TP<TN T0>
    constexpr T0 radians( T0 angle ) { return angle * pi< T0 > / T0{ 180 }; }

    constexpr auto operator ""_rad( unsigned long long angle ) { return radians< double >( angle ); }
     

  }

}
