#pragma once

#include <cmath>
#include "common.h"


namespace lib {

  namespace m {

    #define $vec_fn( $0, $1 ) \
      TP< TN T0, ssize_t... NN > \
      constexpr auto $0( vec_t< T0, NN...> left, vec_t< T0, NN... > right ) { \
        return vec_t< T0, NN... >{ left[ NN ] $1 right[ NN ]... }; \
      } 

    #define $vec_op( $0, $1 ) \
      TP< TN T0, ssize_t... NN > \
      constexpr auto operator $1( vec_t< T0, NN...> left, vec_t< T0, NN... > right ) { \
        return $0( left, right ); \
      }
    
    $vec_fn( add, + );
    $vec_fn( sub, - );
    $vec_fn( mul, * );
    $vec_fn( div, / );

    $vec_op( add, + );
    $vec_op( sub, - );
    $vec_op( mul, * );
    $vec_op( div, / );


    TP< TN T0 >
    constexpr auto add( T0 left ) { return left; }

    TP< TN T0, TN... TT >
    constexpr auto add( T0 left, TT... args ) { return left + add( args... ); }


    TP< TN T0, ssize_t... NN > 
    constexpr auto dot( vec_t< T0, NN...> left, vec_t< T0, NN... > right ) { 

        return add( left[ NN ] * right[ NN ]... );
    }

    TP< TN T0, ssize_t... NN > 
    constexpr T0 length( vec_t< T0, NN... > v ) { return sqrt( dot( v, v ) ); }

    TP< TN T0, ssize_t... NN > 
    constexpr auto normalize( vec_t< T0, NN...> v ) { 

        return v / vec_t< T0, NN... >{ length( v ) };
    }


    #define $mat_fn( $0, $1 ) \
      TP< TN T0, ssize_t... NN > \
      constexpr auto $0( mat_t< T0, NN...> left, mat_t< T0, NN... > right ) { \
        return mat_t< T0, NN... >{ left[ NN ] $1 right[ NN ]... }; \
      } 

    #define $mat_op( $0, $1 ) \
      TP< TN T0, ssize_t... NN > \
      constexpr auto operator $1( mat_t< T0, NN...> left, mat_t< T0, NN... > right ) { \
        return $0( left, right ); \
      }
    
    $mat_fn( add, + );
    $mat_fn( sub, - );
    $mat_fn( mul, * );
    $mat_fn( div, / );

    $mat_op( add, + );
    $mat_op( sub, - );
    $mat_op( mul, * );
    $mat_op( div, / );


    TP< TN T0, ssize_t... NN > 
    constexpr auto operator*( mat_t< T0, NN... > left, vec_t< T0, NN... > right ) {
      return vec_t< T0, NN... >{ dot( left[ NN ], right )... };
    }

    TP< TN T0, ssize_t... NN > 
    constexpr auto operator*( vec_t< T0, NN... > left, mat_t< T0, NN... > right ) {
      return vec_t< T0, NN... >{ add( vec_t< T0, NN... >{ left[ NN ] } * right[ NN ] ... ) };
    }



  }

}




