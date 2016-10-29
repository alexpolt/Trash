#pragma once

#include "lib/macros.h"
#include "lib/types.h"


namespace lib {

    namespace math {

    constexpr double pi = 3.14159265358979;
    constexpr double pi12 = .5 * pi;
    constexpr double pi34 = pi + pi12;
    constexpr double pi2 = pi + pi;

    constexpr float pif = pi;
    constexpr float pi12f = pi12;
    constexpr float pi34f = pi34;
    constexpr float pi2f = pi2;

    constexpr double radians( double angle ) { return angle * pi / double( 180 ); }

    constexpr float radiansf( float angle ) { return angle * pif / float( 180 ); }

    constexpr auto operator ""_rad( unsigned long long angle ) { return radians( angle ); }
     

  }

}
