#pragma once

#include "lib/macros.h"
#include "lib/assert.h"
#include "lib/types.h"
#include "lib/range.h"
#include "lib/to-string.h"
#include "constant.h"


namespace lib {

  namespace math {


    TP<TN T, ssize_t... NN>
    struct vec_t {
    
      using value_type = T;

      static constexpr ssize_t _size = sizeof...( NN );
      
      static constexpr bool is_math_vector = true;

      constexpr vec_t() {}

      TP<TN U, ssize_t... MM, TN = enable_if_t< sizeof...( NN ) <= sizeof...( MM ) >>
      explicit constexpr vec_t( vec_t< U, MM... > const& other ) : _data{ value_type( other[ NN ] )... } { }

      TP<TN U, ssize_t... MM, TN = enable_if_t< ( sizeof...( NN ) > sizeof...( MM ) ) >, TN = void>
      explicit constexpr vec_t( vec_t< U, MM... > const& other ) : _data{ value_type( other[ MM ] )... } { }

      TP<TN U>
      explicit constexpr vec_t( U arg ) : _data{ ( (void)NN, value_type( arg ) )... } { }

      TP<TN... UU>
      explicit constexpr vec_t( UU... args ) : _data{ value_type( args )... } { }

      TP<TN U, ssize_t... MM>
      constexpr vec_t( vec_t< U, MM... > const& other ) : _data{ other[ MM ]... } { }

      constexpr auto& operator[]( ssize_t idx ) { check_bounds( idx ); return _data[ idx ]; }
      constexpr auto& operator[]( ssize_t idx ) const { check_bounds( idx ); return _data[ idx ]; }


      TP<TN U, ssize_t... MM, TN = enable_if_t< sizeof...( NN ) <= sizeof...( MM ) >>
      constexpr auto& operator=( vec_t< U, MM... >& right ) {

        char dummy[] = { ( $this[ NN ] = right[ NN ], '\0' )... }; (void) dummy;
        return $this;
      }

      TP<TN U, ssize_t... MM, TN = enable_if_t< ( sizeof...( NN ) > sizeof...( MM ) ) >, TN = void>
      constexpr auto& operator=( vec_t< U, MM... >& right ) {

        char dummy[] = { ( $this[ MM ] = right[ MM ], '\0' )... }; (void) dummy;
        return $this;
      }


      constexpr auto& data() { return _data; }
      constexpr auto& data() const { return _data; }
      constexpr auto  size() const { return _size; }

      constexpr void check_bounds( ssize_t idx ) const { $assert( idx < size(), "out of bounds" ); }


      value_type _data[ _size ]{};
    };


    TP<TN T0, TN T1> 
    struct define_vector_t;

    TP<TN T, ssize_t... NN> 
    struct define_vector_t< T, lib::index_list< NN...> > { using type = vec_t< T, NN... >; };

    TP<TN T, ssize_t N>
    using vec = typename define_vector_t< T, lib::index_list_t< N > >::type;

    #define $define_vec( $0, $1, $2 ) \
    using $paste( vec, $paste( $0, $1 ) ) = typename define_vector_t< $2, lib::index_list_t< $0 > >::type;

    // vec2f, vec3f, vec4f, vec2d, vec3d, vec4d, vec2i, vec3i, vec4i

    $define_vec( 2, f, float )
    $define_vec( 3, f, float )
    $define_vec( 4, f, float )

    $define_vec( 2, d, double )
    $define_vec( 3, d, double )
    $define_vec( 4, d, double )

    $define_vec( 2, i, int )
    $define_vec( 3, i, int )
    $define_vec( 4, i, int )

    TP<TN T>
    using quat = vec< T, 4 >;


    TP<TN T, ssize_t... NN>
    struct mat_t {

      static constexpr ssize_t _size = sizeof...( NN );

      using value_type = vec< T, _size >;

      constexpr mat_t() {}
      
      TP<TN U, ssize_t... MM, TN = enable_if_t< sizeof...( NN ) <= sizeof...( MM ) >>
      explicit constexpr mat_t( mat_t< U, MM... > const& other ) : _data{ other[ NN ]... } { }

      TP<TN U, ssize_t... MM, TN = enable_if_t< ( sizeof...( NN ) > sizeof...( MM ) ) >, TN = void>
      explicit constexpr mat_t( mat_t< U, MM... > const& other ) : _data{ other[ MM ]... } { }

      TP<TN U, TN = enable_if_t< is_primitive_v< U > >>
      explicit constexpr mat_t( U arg ) {
        
        for( auto v : range{ 0, _size } )
          for( auto d : range{ 0, _size } ) 
            _data[ v ][ d ] = ( v == d ) ? arg : 0;
      }

      TP<TN... UU, TN = enable_if_t< type_first_t< UU... >::is_math_vector >>
      explicit constexpr mat_t( UU... args ) : _data{ value_type{ args }... } { }

      TP<TN... UU, TN = enable_if_t< is_primitive_v< type_first_t< UU... > > >, TN = void>
      explicit constexpr mat_t( UU... args ) { 

        static_assert( sizeof...( args ) <= _size * _size, "too many arguments" );

        type_first_t< UU... > data_args[]{ args... };

        for( auto v : range{ 0, _size } )
          for( auto d : range{ 0, _size } ) 
            if( uint( v * _size + d ) < sizeof...( args ) ) 
                 _data[ v ][ d ] = data_args[ v * _size + d ];
            else _data[ v ][ d ] =  0;
       }

      TP<TN U, ssize_t... MM>
      constexpr mat_t( mat_t< U, MM... > const& other ) : _data{ other[ MM ]... } { }

      constexpr auto& operator[]( ssize_t idx ) { check_bounds( idx ); return _data[ idx ]; }
      constexpr auto& operator[]( ssize_t idx ) const { check_bounds( idx ); return _data[ idx ]; }

      TP<TN U, ssize_t... MM, TN = enable_if_t< sizeof...( NN ) <= sizeof...( MM ) >>
      constexpr auto& operator=( mat_t< U, MM... >& right ) {

        char dummy[] = { ( $this[ NN ] = right[ NN ], '\0' )... }; (void) dummy;
        return $this;
      }

      TP<TN U, ssize_t... MM, TN = enable_if_t< ( sizeof...( NN ) > sizeof...( MM ) ) >, TN = void>
      constexpr auto& operator=( mat_t< U, MM... >& right ) {

        char dummy[] = { ( $this[ MM ] = right[ MM ], '\0' )... }; (void) dummy;
        return $this;
      }

      constexpr auto column( ssize_t idx ) const { 

        check_bounds( idx ); 
        
        return value_type{ _data[ NN ][ idx ]... };
      }

      constexpr auto& data() { return _data; }
      constexpr auto& data() const { return _data; }
      constexpr auto  size() const { return _size; }

      constexpr void check_bounds( ssize_t idx ) const { $assert( idx < size(), "out of bounds" ); }


      value_type _data[ _size ];
    };


    TP<TN T0, TN T1> 
    struct define_mat_t;

    TP<TN T, ssize_t... NN> 
    struct define_mat_t< T, lib::index_list< NN...> > { using type = mat_t< T, NN... >; };

    TP<TN T, ssize_t N>
    using mat = typename define_mat_t< T, lib::index_list_t< N > >::type;

    #define $define_mat( $0, $1, $2 ) \
    using $paste( mat, $paste( $0, $1 ) ) = typename define_mat_t< $2, lib::index_list_t< $0 > >::type;

    // mat2f, mat3f, mat4f, mat2d, mat3d, mat4d, mat2i, mat3i, mat4i

    $define_mat( 2, f, float )
    $define_mat( 3, f, float )
    $define_mat( 4, f, float )

    $define_mat( 2, d, double )
    $define_mat( 3, d, double )
    $define_mat( 4, d, double )

    $define_mat( 2, i, int )
    $define_mat( 3, i, int )
    $define_mat( 4, i, int )


  }

}



