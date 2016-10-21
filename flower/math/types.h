#pragma once

#include "../lib/macros.h"
#include "../lib/assert.h"
#include "../lib/types.h"

#include "constant.h"


namespace lib {


  namespace m {


    TP<TN T0, ssize_t... NN>
    struct vec_t {
    
      using value_type = T0;

      static constexpr ssize_t _size = sizeof...( NN );

      TP<TN U0>
      constexpr explicit vec_t( U0 arg ) : _data{ ( (void)NN, value_type( arg ) )... } { }

      TP<TN... UU>
      constexpr explicit vec_t( UU... args ) : _data{ value_type( args )... } { }

      constexpr auto& operator[]( ssize_t idx ) { check_bounds( idx ); return _data[ idx ]; }
      constexpr auto& operator[]( ssize_t idx ) const { check_bounds( idx ); return _data[ idx ]; }

      constexpr auto& data() { return _data; }
      constexpr auto& data() const { return _data; }
      constexpr auto  size() const { return _size; }

      constexpr void check_bounds( ssize_t idx ) const { $assert( idx < size(), "out of bounds" ); }


      value_type _data[ _size ];
    };


    TP<TN T0, TN T1> 
    struct define_vector_t;

    TP<TN T0, ssize_t... NN> 
    struct define_vector_t< T0, lib::index_list< NN...> > { using type = vec_t< T0, NN... >; };

    TP<TN T0, ssize_t N0>
    using vec = typename define_vector_t< T0, lib::index_list_t< N0 > >::type;

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


    TP<TN T0, ssize_t... NN>
    struct mat_t {

      static constexpr ssize_t _size = sizeof...( NN );

      using value_type = vec< T0, _size >;

      TP<TN U0>
      constexpr explicit mat_t( U0 arg ) : _data{ ( (void)NN, value_type{ arg } )... } { }

      TP<TN... UU>
      constexpr explicit mat_t( UU... args ) : _data{ value_type{ args }... } { }

      constexpr auto& operator[]( ssize_t idx ) { check_bounds( idx ); return _data[ idx ]; }
      constexpr auto& operator[]( ssize_t idx ) const { check_bounds( idx ); return _data[ idx ]; }

      constexpr auto& data() { return _data; }
      constexpr auto& data() const { return _data; }
      constexpr auto  size() const { return _size; }

      constexpr void check_bounds( ssize_t idx ) const { $assert( idx < size(), "out of bounds" ); }


      value_type _data[ _size ];
    };


    TP<TN T0, TN T1> 
    struct define_mat_t;

    TP<TN T0, ssize_t... NN> 
    struct define_mat_t< T0, lib::index_list< NN...> > { using type = mat_t< T0, NN... >; };

    TP<TN T0, ssize_t N0>
    using mat = typename define_mat_t< T0, lib::index_list_t< N0 > >::type;

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





