#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "lib/to-string.h"
#include "event/common.h"


namespace lib {

  namespace os {


    namespace events {

      using events_it = event::events<>::iterator;

      TP<TN...> events_it key_up{ event::create( "key_up" ) };
      TP<TN...> events_it key_down{ event::create( "key_down" ) };
      TP<TN...> events_it scroll{ event::create( "scroll" ) };
      TP<TN...> events_it window_paint{ event::create( "window_paint" ) };
      TP<TN...> events_it window_close{ event::create( "window_close" ) };
      TP<TN...> events_it window_resize{ event::create( "window_resize" ) };
      TP<TN...> events_it input_message{ event::create( "input_message" ) };
      TP<TN...> events_it window_activate{ event::create( "window_activate" ) };
      TP<TN...> events_it exit{ event::create( "exit" ) };
      TP<TN...> events_it mouse_up{ event::create( "mouse_up" ) };
      TP<TN...> events_it mouse_down{ event::create( "mouse_down" ) };
      TP<TN...> events_it mouse_move{ event::create( "mouse_move" ) };
      TP<TN...> events_it mouse_rinput{ event::create( "mouse_rinput" ) };
      TP<TN...> events_it monitor{ event::create( "monitor" ) };
      TP<TN...> events_it screensaver{ event::create( "screensaver" ) };

      bool fire( events_it& it, event::event_data& event ) {

        bool result = false;

        for( auto& e : *it ) {

          result = e( event );

          if( not result ) break;
        }

        return result;
      }

    }



  }
}


