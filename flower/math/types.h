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

    TP<ssize_t N0>
    using vecf = typename define_vector_t< float, lib::index_list_t< N0 > >::type;

    using vec2f = typename define_vector_t< float, lib::index_list_t< 2 > >::type;
    using vec3f = typename define_vector_t< float, lib::index_list_t< 3 > >::type;
    using vec4f = typename define_vector_t< float, lib::index_list_t< 4 > >::type;

    TP<ssize_t N0>
    using vecd = typename define_vector_t< double, lib::index_list_t< N0 > >::type;

    TP<ssize_t N0>
    using veci = typename define_vector_t< ssize_t, lib::index_list_t< N0 > >::type;



    TP<TN T0, ssize_t... NN>
    struct mat_t {

      static constexpr ssize_t _size = sizeof...( NN );

      using value_type = vec< T0, _size >;

      TP<TN U0>
      constexpr explicit mat_t( U0 arg ) : _data{ ( (void)NN, value_type{ arg } )... } { }

      TP<TN... UU>
      constexpr explicit mat_t( UU... args ) : _data{ { args }... } { }

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

    TP<ssize_t N0>
    using matf = typename define_mat_t< float, lib::index_list_t< N0 > >::type;

    using mat2f = typename define_mat_t< float, lib::index_list_t< 2 > >::type;
    using mat3f = typename define_mat_t< float, lib::index_list_t< 3 > >::type;
    using mat4f = typename define_mat_t< float, lib::index_list_t< 4 > >::type;

    TP<ssize_t N0>
    using matd = typename define_mat_t< double, lib::index_list_t< N0 > >::type;

    TP<ssize_t N0>
    using mati = typename define_mat_t< ssize_t, lib::index_list_t< N0 > >::type;


  }

}





