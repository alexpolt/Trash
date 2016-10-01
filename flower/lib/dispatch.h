#pragma once

#include "common.h"

namespace lib {

  $t<$n T0, $n... TT>
  struct dispatch {

    T0 fn;

    dispatch() {
      init_t::template init< TT... >();
    }


    struct init_t {

      $t<$n... UU> 
      static void init() {

        char dummy[] { ( set_entry_t< TT >::template set_entry< UU... >(), '\0' )... }; 

        (void) dummy;
      };

    }; 

    $t<$n U0> struct set_entry_t {

      $t<$n... UU> 
      static void set_entry() {

        char dummy[] { 
          ( dispatch_table[ init_index++ ] = { U0::object_id, UU::object_id, dispatcher< U0, UU > }, '\0' ) ...
        };

        (void) dummy;
      }

    };

    typename T0::type_return operator()( value<object> a, value<object> b ) {

      auto id_a = a->get_object_id();
      auto id_b = b->get_object_id();

      for( auto e : dispatch_table ) {
        if( e.id_a == id_a and e.id_b == id_b ) return e.dispatcher( fn, a, b );
      }

      throw $error_dispatch( a->to_string().data(), b->to_string().data() );
    }

    $t<$n U0, $n U1>
    static typename T0::type_return dispatcher( T0 fn, value< object > a, value< object > b ) {

      return fn( a->get_object( U0::tag ),  b->get_object( U1::tag ) );

    }

    using dispatch_f = typename T0::type_return (*)( T0 fn, value<object> a, value<object> b );

    struct dispatch_data {
      unsigned id_a;
      unsigned id_b;
      dispatch_f dispatcher;
    };
 
    static dispatch_data dispatch_table[ sizeof...( TT) * sizeof...( TT ) ];
    static unsigned init_index;

  };


  $t<$n T0, $n... TT> unsigned dispatch< T0, TT... >::init_index;

  $t<$n T0, $n... TT> 
    typename dispatch< T0, TT... >::dispatch_data
      dispatch< T0, TT... >::dispatch_table[ sizeof...( TT) * sizeof...( TT ) ];

 


}


