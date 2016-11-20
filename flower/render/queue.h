#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/owner-ptr.h"
#include "lib/vector.h"
#include "lib/queue.h"
#include "lib/alloc-default.h"
#include "lib/to-string.h"
#include "lib/log.h"
#include "types.h"
#include "render-message.h"


namespace lib {

  namespace render {


    namespace global {

      TP<TN...>
      queue< render_message, 4 > render_queue{ alloc_default::create( "render_queue" ) };

    }


    inline void push( render_message msg ) {

      log::render, "push message ", msg, log::endl;

      global::render_queue<> << move( msg );
    }

    inline auto pop() {

      auto msg = global::render_queue<>.pop();

      log::render, "pop message ", msg, log::endl;

      return msg;
    }

    inline bool available() {

      return global::render_queue<>.size() > 0;
    }


  }
}



