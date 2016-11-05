#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"
#include "lib/value.h"
#include "lib/vector.h"
#include "lib/hash-map.h"
#include "lib/algo.h"
#include "lib/log.h"
#include "lib/sequence.h"

#include "event.h"


namespace lib {


  namespace event {

    TP<TN>
    struct events;
  }

  namespace global {

    TP<TN T = void>
    event::events< T > event_map;
    
  }


  namespace event {

    TP<TN = void>
    struct events {

      using vector_event = vector< value< event > >;

      using event_map = hash_map< cstr, vector_event >;


      eid_t add( event_desc desc, value< event > cb ) {
        
        auto it = _event_map[ desc.name ];

        if( not it ) _event_map.insert( desc.name, vector_event{} );

        else {

          auto it_e = find( *it, [&desc]( value< event >& e ){ return equal( e->get_origin(), desc.origin ); } );

          if( it_e )

            log::warn, $file_line, "event ", desc.name," from ", desc.origin, " was already added", log::endl;
        }

        it->push_back( move( cb ) );

        log::event, "add event#", desc.id, " ", (void*)&it->back(), "( ", desc.name, " ) from ", desc.origin, log::endl;

        return desc.id;
      }


      bool remove( cstr name, eid_t id ) {
        
        auto it = _event_map[ name ];

        if( not it ) {

          log::warn, $file_line, "no events for ", name, log::endl;

          return false;
        }

        auto it_e = find( *it, [id]( value< event >& e ){ return e->get_id() == id;} );

        if( not it_e ) {

          log::warn, $file_line, "event #", id, " in ", name, " not found", log::endl;

          return false;
        }

        log::event, "removed ", *it_e, log::endl;

        it->erase( it_e );

        return true;
      }


      bool fire( cstr name, event_data& event ) {

        auto it = _event_map[ name ];

        if( not it ) {

          log::event, "fire event ", name, ", no events found", log::endl;

          return false;
        }

        for( auto& e : *it ) {

          log::event, "fire ", e, log::endl;

          bool r = e( event );

          if( not r ) {

            log::event, "event returned false", log::endl;

            return false;
          }
        }

        return true;
      }


      event_map _event_map;
    };


    struct event_tag {};


    TP<TN T> 
    eid_t operator+( event_desc desc, T fn ) { 

      auto id = desc.id = global::gen_id< event_tag >();

      auto t = value< event >::create< event_basic< T > >( move( fn ), desc );

      global::event_map<>.add( desc, move( t ) );

      return id;
    }


    #define $event_add( ... ) \
      lib::event::event_desc{ $args_first( __VA_ARGS__ ), $file_line } + \
        [ $args_second( __VA_ARGS__ ) ]( lib::event::event_data& event ) -> bool

    #define $event_remove( $0, $1 ) lib::global::event_map<>.remove( $0, $1 )

    #define $event_fire( $0, $1 ) lib::global::event_map<>.fire( $0, $1 )


  }

}


