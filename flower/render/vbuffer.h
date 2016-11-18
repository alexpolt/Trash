#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/vector.h"


namespace lib {

  namespace render {


    struct vbuffer {

      enum format { };

      vbuffer( vector_f data ) : _data{ data } { }

      vbuffer( vector_f data ) : _data{ data } { }

      auto& data() { return _data; }

      void set_data( vector_b data ) { _data = move( data ); }

      vector_f _data;
    };


  }
}




