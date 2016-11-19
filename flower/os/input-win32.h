#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "lib/to-string.h"
#include "event/common.h"
#include "events.h"


namespace lib {

  namespace os {


    struct input_win32 : nocopy {

      static bool fire_events() {

        MSG msg;

        auto event = event::event_data{};

        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
          
          TranslateMessage( &msg );

          event.data = &msg;

          if( msg.message == WM_QUIT ) {

            log::input, "WM_QUIT", log::endl;

            events::fire( events::exit<>, event );

            return false;

          }
          
          events::fire( events::input_message<>, event );    
        }  

        return true;
      }

    };


  }
}


