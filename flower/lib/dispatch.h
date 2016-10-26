#pragma once

#include "macros.h"
#include "types.h"

namespace lib {


  struct error_dispatch;
  

  TP<TN T0, TN... TT>
  struct dispatch {

    T0 fn;

    dispatch() {

      init< TT... >();
    }


    TP<TN... UU> 
    static void init() {

      char dummy[] { ( set_entry_t< TT >::TP set_entry< UU... >(), '\0' )... }; 

      (void) dummy;
    };
    

    TP<TN U0> struct set_entry_t {

      TP<TN... UU> 
      static void set_entry() {

        char dummy[] { 
          ( _dispatch_table[ _init_index++ ] = { U0::interface_id, UU::interface_id, dispatcher< U0, UU > }, '\0' ) ...
        };

        (void) dummy;
      }

    };

    typename T0::type_return operator()( value<object> a, value<object> b ) {

      auto id_a = a->get_interface_id();
      auto id_b = b->get_interface_id();

      for( auto e : _dispatch_table )

        if( e.id_a == id_a and e.id_b == id_b ) 
          
          return e.dispatcher( fn, a, b );

      $throw $error_dispatch( a->to_string().data(), b->to_string().data() );
    }

    TP<TN U0, TN U1>
    static typename T0::type_return dispatcher( T0 fn, value< object > a, value< object > b ) {

      return fn( a->get_object( U0::tag ),  b->get_object( U1::tag ) );

    }

    using dispatch_f = typename T0::type_return (*)( T0 fn, value<object> a, value<object> b );

    struct dispatch_data {
      iid_t id_a;
      iid_t id_b;
      dispatch_f dispatcher;
    };
 
    static dispatch_data _dispatch_table[ sizeof...( TT) * sizeof...( TT ) ];
    static ssize_t _init_index;

  };


  TP<TN T0, TN... TT> ssize_t dispatch< T0, TT... >::_init_index;

  TP<TN T0, TN... TT> 
    typename dispatch< T0, TT... >::dispatch_data
      dispatch< T0, TT... >::_dispatch_table[ sizeof...( TT) * sizeof...( TT ) ];

 
  struct error_dispatch : error {

    error_dispatch( cstr file, int line, cstr func, cstr msg_a, cstr msg_b ) : error{ file, line, func } {
        
      auto l = strlen( error::_buffer );

      auto ptr = error::get_buffer() + l;

      snprintf( ptr, $length( error::get_buffer() ) - l, ": dispatch failed for ( %s ) and ( %s )", msg_a, msg_b );
    }
  };

  #define $error_dispatch( $0, $1 ) error_dispatch{ __FILE__, __LINE__, __func__, $0, $1 }



}


