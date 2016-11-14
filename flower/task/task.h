#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"


namespace lib {

  namespace task {


    using tid_t = oid_t;

    enum result : int { done, retry };

    enum class type : char { null, phys, gfx, ai, };

    constexpr cstr type_desc[]{ "null", "phys", "gfx", "ai" };

    struct task_desc {

      cstr desc;
      char frame;
      type type;
      tid_t id;
    };


    struct task {

      virtual ~task() { }

      static constexpr ssize_t type_size = $size( task_desc ) + $size( void*[2] );

      virtual result operator()() = 0;

      virtual tid_t get_id() const = 0;

      virtual cstr get_desc() const = 0;

      virtual type get_type() const = 0;

      virtual cstr to_string() const = 0;
    };


    TP<TN T0>
    struct task_basic : task {

      task_basic( T0 fn, task_desc desc ) : _fn{ move( fn ) }, _desc{ desc } { }

      result operator()() override { return _fn(); }

      tid_t get_id() const override { return _desc.id; }

      cstr get_desc() const override { return _desc.desc; }

      type get_type() const override { return _desc.type; }

      cstr to_string() const override { 

        return lib::to_string( "#%d( %s, %s )", _desc.id, type_desc[ _desc.type ], _desc.desc ); 
      }

      T0 _fn;
      task_desc _desc;
    };


  }

}


