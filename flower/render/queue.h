#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/queue.h"
#include "lib/alloc-default.h"
#include "types.h"


namespace lib {

  namespace render {


    namespace global {

      TP<TN T=void>
      queue< render_message, 4 > render_queue{ alloc_default::create( "render_queue" ) };

    }


    inline void push( render_message msg ) {
    
      global::render_queue<> << move( msg );
    }

    inline auto pop() {

      return global::render_queue<>.pop();
    }

    inline bool available() {

      return global::render_queue<>.available() > 0;
    }


  }
}



