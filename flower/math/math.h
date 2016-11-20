#pragma once

#include <cmath>
#include <cfloat>

#include "lib/macros.h"
#include "lib/assert.h"
#include "lib/types.h"
#include "types.h"
#include "constant.h"



namespace lib {

  namespace math {

    #define $vec_fn( $0, $1 ) \
      TP< TN T, ssize_t... NN > \
      constexpr auto $0( vec_t< T, NN...> const& left, vec_t< T, NN... > const& right ) { \
        return vec_t< T, NN... >{ left[ NN ] $1 right[ NN ]... }; \
      } 

    #define $vec_op( $0, $1 ) \
      TP< TN T, ssize_t... NN > \
      constexpr auto operator $1( vec_t< T, NN...> const& left, vec_t< T, NN... > const& right ) { \
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

    TP< TN T, ssize_t... NN >
    constexpr auto& operator+=( vec_t< T, NN... >& left, vec_t< T, NN... > const& right ) {

      return left = add( left, right );
    }


    #define $vec_value_op( $0, $1 ) \
      TP< TN T, ssize_t... NN > \
      constexpr auto operator $1( vec_t< T, NN...> const& left, T value ) { \
        return $0( left, vec_t< T, NN... >{ value } ); \
      } \
      TP< TN T, ssize_t... NN > \
      constexpr auto operator $1( T value, vec_t< T, NN...> const& right ) { \
        return $0( right, vec_t< T, NN... >{ value } ); \
      }

    $vec_value_op( mul, * );
    $vec_value_op( div, / );


    #define $mat_fn( $0, $1 ) \
      TP< TN T, ssize_t... NN > \
      constexpr auto $0( mat_t< T, NN...> const& left, mat_t< T, NN... > const& right ) { \
        return mat_t< T, NN... >{ left[ NN ] $1 right[ NN ]... }; \
      } 

    #define $mat_op( $0, $1 ) \
      TP< TN T, ssize_t... NN > \
      constexpr auto operator $1( mat_t< T, NN...> const& left, mat_t< T, NN... > const& right ) { \
        return $0( left, right ); \
      }
    
    $mat_fn( add, + )
    $mat_fn( sub, - )
    $mat_fn( mul, * )
    $mat_fn( div, / )

    $mat_op( add, + )
    $mat_op( sub, - )

    #define $mat_value_op( $0, $1 ) \
      TP< TN T, ssize_t... NN > \
      constexpr auto operator $1( mat_t< T, NN...> const& left, T value ) { \
        return $0( left, mat_t< T, NN... >{ ( (void)NN, vec_t< T, NN... >{ value } )... } ); \
      } \
      TP< TN T, ssize_t... NN > \
      constexpr auto operator $1( T value, mat_t< T, NN...> const& right ) { \
        return $0( right, mat_t< T, NN... >{ ( (void)NN, vec_t< T, NN... >{ value } )... } ); \
      }

    $mat_value_op( mul, * );
    $mat_value_op( div, / );


    TP<TN T>
    constexpr auto add( T left ) { return left; }

    TP<TN T, TN... TT>
    constexpr auto add( T const& left, TT... args ) { return left + add( args... ); }


    TP<TN T, ssize_t... NN > 
    constexpr auto dot( vec_t< T, NN...> const& left, vec_t< T, NN... > const& right ) { 

        return add( left[ NN ] * right[ NN ]... );
    }

    TP<TN T, ssize_t... NN, TN = enable_if_t< sizeof...( NN ) == 2 >, TN = void> 
    constexpr auto cross( vec_t< T, NN...> const& l, vec_t< T, NN... > const& r ) { 

        return l[0] * r[1] - l[1] * r[0];
    }

    TP<TN T, ssize_t... NN, TN = enable_if_t< sizeof...( NN ) == 3 >> 
    constexpr auto cross( vec_t< T, NN...> const& l, vec_t< T, NN... > const& r ) { 

        return vec_t< T, NN... >{ l[1]*r[2]-l[2]*r[1], l[0]*r[2]-l[2]*r[0], l[0]*r[1]-l[1]*r[0] };
    }


    TP<TN T, ssize_t... NN > 
    constexpr auto operator*( mat_t< T, NN... > const& left, vec_t< T, NN... > const& right ) {

      return vec_t< T, NN... >{ dot( left[ NN ], right )... };
    }

    TP<TN T, ssize_t... NN > 
    constexpr auto operator*( vec_t< T, NN... > const& left, mat_t< T, NN... > const& right ) {

      return vec_t< T, NN... >{ add( vec_t< T, NN... >{ left[ NN ] } * right[ NN ] ... ) };
    }

    TP<TN T, ssize_t... NN, TN... TT > 
    constexpr auto dot_helper( vec_t< T, NN...> const& left, TT... args ) { 

      return vec_t< T, NN... >{ dot( left, args )... };
    }

    TP<TN T, ssize_t... NN > 
    constexpr auto operator*( mat_t< T, NN...> const& left, mat_t< T, NN... > const& right ) { 

      return mat_t< T, NN... >{ dot_helper( left[ NN ], right.column( NN )...) ... };
    }


    TP<TN T>
    constexpr auto abs( T a ) { return a < 0 ? -a : a; }

    TP<TN T, ssize_t... NN >
    constexpr auto abs( vec_t< T, NN... > const& value ) { return vec_t< T, NN... >{ abs( value[ NN ] )... }; }

    TP<TN T>
    constexpr auto sign( T a ) { return a < 0 ? T( -1 ) : T( 1 ); }

    TP<TN T, ssize_t... NN >
    constexpr auto sign( vec_t< T, NN... > const& value ) { return vec_t< T, NN... >{ sign( value[ NN ] )... }; }

    TP<TN T>
    constexpr auto max( T arg0 ) { return arg0; }

    TP<TN T, TN... TT> 
    constexpr auto max( T arg0, TT... args ) { 

      auto r = max( args... );

      return arg0 > r ? arg0 : r; 
    }

    TP<TN T>
    constexpr auto min( T arg0 ) { return arg0; }

    TP<TN T, TN... TT>
    constexpr auto min( T arg0, TT const&... args ) { 

      auto r = min( args... );

      return arg0 > r ? r : arg0; 
    }

    TP<TN T, ssize_t... NN>
    constexpr auto max( vec_t< T, NN... > const& value ) { return max( value[ NN ]... ); }

    TP<TN T, ssize_t... NN>
    constexpr auto min( vec_t< T, NN... > const& value ) { return min( value[ NN ]... ); }

    TP<TN T>
    constexpr auto mod( T a, T b ) { return a % b; }
    constexpr auto mod( float a, float b ) { return a - b * int( a / b ); }
    constexpr auto mod( double a, double b ) { return a - b * (long long int)( a / b ); }

    constexpr auto frac( float value ) { return value - float( int( abs( value ) ) ); }
    constexpr auto frac( double value ) { return value - double( (long long int)( abs( value ) ) ); }

    constexpr auto trunc( float value ) { return float( int( value ) ); }
    constexpr auto trunc( double value ) { return double( (long long int)( value ) ); }

    constexpr auto round( float value ) {

      return value > 0 ? trunc( value + 0.5f ) : trunc( value - 0.5f );
    }
    
    constexpr auto round( double value ) {

      return value > 0 ? trunc( value + 0.5 ) : trunc( value - 0.5 );
    }

    constexpr auto round_precise( float value ) {

      auto n2 = (int)( value * 2.f );
      auto bit = n2 & 0x1;
      bit = n2 >> ( 8 * $size( int ) - 1 ) ? -bit : bit;
      return  n2/2 + bit;
    }

    constexpr auto round_precise( double value ) {

      auto n2 = (long long int)( value * 2. );
      auto bit = n2 & 0x1;
      bit = n2 >> ( 8 * $size( long long int ) - 1 ) ? -bit : bit;
      return  n2/2 + bit;
    }

    constexpr auto sqrt_cexpr( double value, double guess = 1.0, double n = 0 ) {

      if( value < 0. ) $throw "sqrt of a negative number";

      if( value == 0. ) return 0.;
      auto d = guess * guess;
      if( d == value or math::abs( d - value ) < 2. * value * DBL_EPSILON ) return guess;
      if( n > 15 ) return guess;
      
      return sqrt_cexpr( value, 0.5 * ( guess + value / guess ), ++n );
    }

    TP<ssize_t... NN > 
    constexpr auto sqrt_cexpr( vec_t< double, NN... > value ) {
      
      return vec_t< double, NN... >{ sqrt_cexpr( value[ NN ] )... };
    }

    #define $sqrt( ... ) lib::math::sqrt_cexpr( __VA_ARGS__ );

    constexpr auto angle_adjust( double& angle, bool cos ) {

      double s = cos ? 1.0 : sign( angle );

      angle = abs( angle );

      while( angle > pi2 ) angle -= pi2;

      if( angle > pi34 ) { angle = pi2 - angle; if( not cos ) s = -1.0 * s; }
      else if( angle > pi ) { angle -= pi; s = -1.0 * s; }
      else if( angle > pi12 ) { angle = pi - angle; if( cos ) s = -1.0 * s; }

      return s;
    }

    constexpr auto cos_cexpr_( double angle, double x, double sign, double fact, double n ) { 

      if( n > 15 ) return .0;

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

    TP<TN T, ssize_t... NN >
    constexpr T norm( vec_t< T, NN... > const& v ) { return sqrt( dot( v, v ) ); }

    TP<TN T, ssize_t... NN >
    constexpr auto normalize( vec_t< T, NN...> const& v ) { 

        return v / norm( v );
    }

    TP<TN T, ssize_t... NN >
    constexpr auto normalize( mat_t< T, NN...> const& m ) {

        return mat_t< T, NN... > { m[ NN ] / norm( m[ NN ] )... };
    }

    TP<TN T, ssize_t... NN >
    constexpr T norm_cexpr( vec_t< T, NN... > const& v ) { return sqrt_cexpr( dot( v, v ) ); }

    #define $norm( ... ) lib::math::norm_cexpr( __VA_ARGS__ )

    TP<TN T, ssize_t... NN >
    constexpr T normalize_cexpr( vec_t< T, NN...> const& v ) { 

        return v / norm_cexpr( v );
    }

    TP<TN T, ssize_t... NN > 
    constexpr auto normalize_cexpr( mat_t< T, NN...> const& m ) { 

        return mat_t< T, NN... > { m[ NN ] / norm_cexpr( m[ NN ] )... };
    }

    #define $normalize( ... ) lib::math::normalize_cexpr( __VA_ARGS__ )


    TP<TN T, ssize_t... NN >
    constexpr auto qmul( vec_t< T, NN... > const& l, vec_t< T, NN... > const& right ) {

      auto m = mat< T, 4 > {

        vec_t< T, NN... >{ l[0], -l[1], -l[2], -l[3] },
        vec_t< T, NN... >{ l[1],  l[0], -l[3],  l[2] },
        vec_t< T, NN... >{ l[2],  l[3],  l[0], -l[1] },
        vec_t< T, NN... >{ l[3], -l[2],  l[1],  l[0] },
      };

      return m * right;
    }

    TP<TN T, ssize_t... NN >
    constexpr auto conj( vec_t< T, NN... > const& v ) {

      return quat< T >{ v[ 0 ], -v[ 1 ], -v[ 2 ], -v[ 3 ] };
    }

    TP<TN T, ssize_t... NN >
    constexpr auto qdiv( vec_t< T, NN... > const& left, vec_t< T, NN... > const& right ) {

      return qmul( left, conj( right ) ) / vec_t< T, NN... >{ dot( right, right ) };
    }

    TP<TN T, ssize_t... NN >
    constexpr auto qsqrt( vec_t< T, NN... > const& q ) {

      auto x = T( 0.5 ) * q;

      range{ 0, 10 } $do {

       x = T( 0.5 ) * ( x + qdiv( q, x ) );

      };

      return x;
    }



  }

}


