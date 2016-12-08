#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"
#include "os/vkey.h"
#include "os/action.h"
#include "os/types.h"


namespace lib {


  namespace event {


    using eid_t = oid_t;


    struct event_data {

      void* data;

      short x;

      short y;

      os::action action;

      os::vkey key;

      os::vmod mod;
    };


    struct event_desc {

      cstr name;

      cstr desc;

      eid_t id;
    };


    struct event : nocopy_vbase {

      static constexpr ssize_t type_size = $size( event_desc ) + $size( void*[2] );

      virtual bool operator()( event_data& event ) = 0;

      virtual eid_t get_id() const = 0;

      virtual cstr get_desc() const = 0;

      virtual cstr to_string() const = 0;
    };


    TP<TN T>
    struct event_basic : event {

      event_basic( T fn, event_desc desc ) : _fn{ move( fn ) }, _desc{ desc } { }

      bool operator()( event_data& event ) override { return _fn( event ); }

      eid_t get_id() const override { return _desc.id; }

      cstr get_desc() const override { return _desc.desc; }

      cstr to_string() const override { 

        return lib::to_string( "%s, #%d( %s )", _desc.name, _desc.id, _desc.desc ); 
      }

      T _fn;
      event_desc _desc;
    };


  }

}



