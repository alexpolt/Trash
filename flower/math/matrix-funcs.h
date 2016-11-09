#pragma once

#include <cmath>

#include "lib/macros.h"
#include "lib/types.h"

#include "constant.h"
#include "types.h"
#include "math.h"


namespace lib {

  namespace math {


    TP<TN T>
    auto rotx( T angle ) {

      return mat< T, 3 >{
        
        vec< T, 3 >{ 1, 0, 0 },
        vec< T, 3 >{ 0,  cos( angle ), sin( angle ) },
        vec< T, 3 >{ 0, -sin( angle ), cos( angle ) }
      };
    }

    TP<TN T>
    auto roty( T angle ) {

      return mat< T, 3 >{

        vec< T, 3 >{ cos( angle ), 0, -sin( angle ) },
        vec< T, 3 >{ 0, 1, 0 },
        vec< T, 3 >{ sin( angle ), 0,  cos( angle ) }
      };
    }

    TP<TN T>
    auto rotz( T angle ) {

      return mat< T, 3 >{ 

        vec< T, 3 >{ cos( angle ), -sin( angle ), 0 },
        vec< T, 3 >{ sin( angle ),  cos( angle ), 0 },
        vec< T, 3 >{ 0, 0, 1 }
      };
    }

    TP<TN T>
    constexpr auto $rotx( T angle ) {

      return mat< T, 3 >{
        
        vec< T, 3 >{ 1, 0, 0 },
        vec< T, 3 >{ 0,  $cos( angle ), $sin( angle ) },
        vec< T, 3 >{ 0, -$sin( angle ), $cos( angle ) }
      };
    }

    TP<TN T>
    constexpr auto $roty( T angle ) {

      return mat< T, 3 >{

        vec< T, 3 >{ $cos( angle ), 0, -$sin( angle ) },
        vec< T, 3 >{ 0, 1, 0 },
        vec< T, 3 >{ $sin( angle ), 0,  $cos( angle ) }
      };
    }

    TP<TN T>
    constexpr auto $rotz( T angle ) {

      return mat< T, 3 >{ 

        vec< T, 3 >{ $cos( angle ), -$sin( angle ), 0 },
        vec< T, 3 >{ $sin( angle ),  $cos( angle ), 0 },
        vec< T, 3 >{ 0, 0, 1 }
      };
    }


  }

}


