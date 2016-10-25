#pragma once

#include <cmath>

#include "../lib/macros.h"
#include "../lib/assert.h"
#include "../lib/types.h"

#include "types.h"
#include "constant.h"



namespace lib {

  namespace math {

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
    
    $vec_fn( add, + )
    $vec_fn( sub, - )
    $vec_fn( mul, * )
    $vec_fn( div, / )

    $vec_op( add, + )
    $vec_op( sub, - )
    $vec_op( mul, * )
    $vec_op( div, / )



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
    
    $mat_fn( add, + )
    $mat_fn( sub, - )
    $mat_fn( mul, * )
    $mat_fn( div, / )

    $mat_op( add, + )
    $mat_op( sub, - )
    $mat_op( mul, * )
    $mat_op( div, / )


    TP< TN T0 >
    constexpr auto add( T0 left ) { return left; }

    TP< TN T0, TN... TT >
    constexpr auto add( T0 left, TT... args ) { return left + add( args... ); }


    TP< TN T0, ssize_t... NN > 
    constexpr auto dot( vec_t< T0, NN...> left, vec_t< T0, NN... > right ) { 

        return add( left[ NN ] * right[ NN ]... );
    }

    TP< TN T0, ssize_t... NN > 
    constexpr auto operator*( mat_t< T0, NN... > left, vec_t< T0, NN... > right ) {
      return vec_t< T0, NN... >{ dot( left[ NN ], right )... };
    }

    TP< TN T0, ssize_t... NN > 
    constexpr auto operator*( vec_t< T0, NN... > left, mat_t< T0, NN... > right ) {
      return vec_t< T0, NN... >{ add( vec_t< T0, NN... >{ left[ NN ] } * right[ NN ] ... ) };
    }

    TP<TN T0>
    constexpr auto abs( T0 a ) { return a < 0 ? -a : a; }

    TP< TN T0, ssize_t... NN > 
    constexpr auto abs( vec_t< T0, NN... > value ) { return vec_t< T0, NN... >{ abs( value[ NN ] )... }; }

    TP<TN T0>
    constexpr auto sign( T0 a ) { return a < 0 ? T0( -1.0 ) : T0( 1.0 ); }

    TP< TN T0, ssize_t... NN > 
    constexpr auto sign( vec_t< T0, NN... > value ) { return vec_t< T0, NN... >{ sign( value[ NN ] )... }; }

    constexpr auto sqrt_cexpr( double value, double guess = 1.0, double n = 0 ) {

      if( value == .0 ) return .0;
      if( abs( guess * guess - value ) < 0.000000000005 ) return guess;
      if( n > 17 ) return guess;
      
      return value > .0 ? sqrt_cexpr( value, 0.5 * ( guess + value / guess ), ++n ) : throw "negative value";
    }

    TP<ssize_t... NN > 
    constexpr auto sqrt_cexpr( vec_t< double, NN... > value ) {
      
      return vec_t< double, NN... >{ sqrt_cexpr( value[ NN ] )... };
    }

    #define $sqrt( ... ) lib::math::sqrt_cexpr( __VA_ARGS__ );

    constexpr auto angle_adjust( double &angle, bool cos ) {

      double s = cos ? 1.0 : sign( angle );

      angle = abs( angle );

      while( angle > pi2< double > ) angle -= pi2< double >;

      if( angle > pi34< double > ) { angle = pi2< double > - angle; if( not cos ) s = -1.0 * s; }
      else if( angle > pi< double > ) { angle -= pi< double >; s = -1.0 * s; }
      else if( angle > pi12< double > ) { angle = pi< double > - angle; if( cos ) s = -1.0 * s; }

      return s;
    }

    constexpr auto cos_cexpr_( double angle, double x, double sign, double fact, double n ) { 

      if( n > 17 ) return .0;

      auto x2 = x * angle * angle;
      auto sign2 = -sign;
      auto  fact2 = fact * n * (n+1);

      return sign2 * x2 / fact2 + cos_cexpr_( angle, x2, sign2, fact2, n + 2 );
    };

    constexpr auto cos_cexpr( double angle ) { 

      double sign = angle_adjust( angle, true );

      return sign * ( 1 + cos_cexpr_( angle, 1.0, 1.0, 1.0, 1.0 ) );
    };

    #define $cos( ... ) lib::math::cos_cexpr( __VA_ARGS__ )

    constexpr auto sin_cexpr( double angle ) {

      double sign = angle_adjust( angle, false );

      return sign * ( sqrt_cexpr( 1.0 - cos_cexpr( angle ) * cos_cexpr( angle ) ) );
    }

    #define $sin( ... ) lib::math::sin_cexpr( __VA_ARGS__ )

    TP< TN T0, ssize_t... NN > 
    constexpr T0 length( vec_t< T0, NN... > v ) { return sqrt( dot( v, v ) ); }

    TP< TN T0, ssize_t... NN > 
    constexpr auto normalize( vec_t< T0, NN...> v ) { 

        return v / vec_t< T0, NN... >{ length( v ) };
    }

    TP< TN T0, ssize_t... NN > 
    constexpr T0 length_cexpr( vec_t< T0, NN... > v ) { return sqrt_cexpr( dot( v, v ) ); }

    #define $vector_length( ... ) lib::math::length_cexpr( __VA_ARGS__ )

    TP< TN T0, ssize_t... NN > 
    constexpr auto normalize_cexpr( vec_t< T0, NN...> v ) { 

        return v / vec_t< T0, NN... >{ length_cexpr( v ) };
    }

    #define $normalize( ... ) lib::math::normalize_cexpr( __VA_ARGS__ )


  }

}


