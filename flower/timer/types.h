#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"
#include "lib/time.h"


namespace lib {


  namespace timer {


    using tid_t = oid_t;


    struct timeout_desc {

      cstr desc;

      lib::time time;

      tid_t id;
      
      lib::time timeout;
    };


    struct timeout : nocopy_vbase {

      static constexpr ssize_t type_size = $size( timeout_desc ) + $size( void*[2] );

      virtual void operator()() = 0;

      virtual tid_t get_id() const = 0;

      virtual cstr get_desc() const = 0;

      virtual time get_time() const = 0;

      virtual time get_timeout() const = 0;

      virtual void set_timeout( time ) = 0;

      virtual cstr to_string() const = 0;
    };


    TP<TN T0>
    struct timeout_basic : timeout {

      timeout_basic( T0 fn, timeout_desc desc ) : _fn{ move( fn ) }, _desc{ desc } { }

      void operator()() override { _fn(); }

      tid_t get_id() const override { return _desc.id; }

      cstr get_desc() const override { return _desc.desc; }

      time get_time() const override { return _desc.time; }

      time get_timeout() const override { return _desc.timeout; }

      void set_timeout( time t ) override { _desc.timeout = t; }

      cstr to_string() const override { 

        return lib::to_string( "timer #%d( %s, %u.%0.3u )", 
                                _desc.id, _desc.desc, _desc.time.seconds(), _desc.time.mseconds() ); 
      }

      T0 _fn;
      timeout_desc _desc;
    };


  }

}



