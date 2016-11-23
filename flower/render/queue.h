#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/owner-ptr.h"
#include "lib/vector.h"
#include "lib/queue.h"
#include "lib/alloc-default.h"
#include "lib/to-string.h"
#include "lib/log.h"
#include "render-message.h"


namespace lib {

  namespace render {


    namespace global {

      TP<TN...>
      queue< render_message, 4 > render_queue{ alloc_default::create( "render_queue" ) };

    }

    TP<TN T>
    inline void push( T msg ) {

      auto rmsg = create_message< T >( move( msg ) );

      global::render_queue<> << move( rmsg );
    }

    inline auto pop() {

      auto msg = global::render_queue<>.pop();

      return msg;
    }

    inline bool available() {

      return global::render_queue<>.size() > 0;
    }


  }
}



