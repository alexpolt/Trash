#pragma once

#include "../lib/macros.h"
#include "../lib/types.h"
#include "../lib/sequence.h"
#include "../lib/object.h"
#include "../lib/to-string.h"

#include "result.h"


namespace lib {

  namespace task {

    using tid_t = oid_t;


    struct task_desc {

      cstr desc;

      tid_t id;
    };

    struct task : object {

      static constexpr ssize_t type_size = $size( task_desc ) + $size( void*[2] );

      virtual result operator()() = 0;

      virtual tid_t get_id() const = 0;

    };


    TP<TN T0>
    struct task_t : task {

      $object( task_t );

      task_t( T0 fn, task_desc desc ) : _fn{ move( fn ) }, _desc{ desc } { }

      result operator()() override { return _fn(); }

      tid_t get_id() const override { return _desc.id; }

      cstr to_string() const override { return lib::to_string( "task #%d: %s", _desc.id, _desc.desc ); }

      T0 _fn;
      task_desc _desc;
    };


  }

}


