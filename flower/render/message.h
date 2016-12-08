#pragma once

#include "lib/macros.h"
#include "lib/types.h"


namespace lib {

  namespace render {

  namespace message {


    enum type : int {

      null, clear, vbuffer, size
    };

    static cstr get_desc( type type ) {

      static cstr type_desc[ type::size ]{ "null", "clear", "vbuffer", };

      return type_desc[ type ];
    }

    struct base : nocopy_vbase {

      virtual cstr to_string() const { return "uknown render operation"; }
    };


  }

  namespace msg = message;

  }
}




