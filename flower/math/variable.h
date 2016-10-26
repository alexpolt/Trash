#pragma once

#include "lib/macros.h"
#include "lib/types.h"

#include "constant.h"
#include "types.h"
#include "math.h"


namespace lib {

  namespace math {


    TP<TN T0, ssize_t A0>
    constexpr mat< T0, 3 > rotx
    {
      vec< T0, 3 >{ 1, 0, 0 },
      vec< T0, 3 >{ 0, sinf( radians< float >( A0 ) ),  cosf( radians< float >( A0 ) ) },
      vec< T0, 3 >{ 0, cosf( radians< float >( A0 ) ), -sinf( radians< float >( A0 ) ) }
    };

    TP<TN T0, ssize_t A0>
    constexpr mat< T0, 3 > roty
    {
      vec< T0, 3 >{ cosf( radians< float >( A0 ) ), 0, -sinf( radians< float >( A0 ) ) },
      vec< T0, 3 >{ 0, 1, 0 },
      vec< T0, 3 >{ sinf( radians< float >( A0 ) ), 0,  cosf( radians< float >( A0 ) ) }
    };

    TP<TN T0, ssize_t A0>
    constexpr mat< T0, 3 > rotz
    { 
      vec< T0, 3 >{ cosf( radians< float >( A0 ) ), -sinf( radians< float >( A0 ) ), 0 },
      vec< T0, 3 >{ sinf( radians< float >( A0 ) ),  cosf( radians< float >( A0 ) ), 0 },
      vec< T0, 3 >{ 0, 0, 1 }
    };


  }

}


