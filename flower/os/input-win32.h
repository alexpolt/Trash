#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "lib/to-string.h"
#include "event/common.h"
#include "input-map-win32.h"
#include "vkey.h"
#include "action.h"
#include "vkey-desc.h"
#ifdef $CONFIG_DEVMODE
  #include "msg-desc.h"
#endif


namespace lib {

  namespace os {


    vmod get_modifiers() {

      vmod mod = vmod::null;

      const short mask = 0x8000;

      if( GetKeyState( (int) vkey::shift ) & mask ) mod = vmod( mod | vmod::shift );
      if( GetKeyState( (int) vkey::control ) & mask ) mod = vmod( mod | vmod::control );
      if( GetKeyState( (int) vkey::menu ) & mask ) mod = vmod( mod | vmod::menu );

      return mod;
    }

    namespace events {

      using events_it = event::events<>::iterator;

      TP<TN=void> events_it key_up{ event::create( "key_up" ) };
      TP<TN=void> events_it key_down{ event::create( "key_down" ) };
      TP<TN=void> events_it scroll{ event::create( "scroll" ) };
      TP<TN=void> events_it window{ event::create( "window" ) };
      TP<TN=void> events_it input_message{ event::create( "input_message" ) };
      TP<TN=void> events_it exit{ event::create( "exit" ) };
      TP<TN=void> events_it mouse_up{ event::create( "mouse_up" ) };
      TP<TN=void> events_it mouse_down{ event::create( "mouse_down" ) };
      TP<TN=void> events_it mouse_move{ event::create( "mouse_move" ) };

      bool fire( events_it& it, event::event_data& event ) {

        bool result = false;

        for( auto& e : *it ) {

          result = e( event );

          if( not result ) return false;
        }

        return result;
      }

    }


    struct input_win32 {


      void operator()() {

        MSG msg;

        auto event = event::event_data{};

        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
          
          #ifdef $CONFIG_DEVMODE
          log::input, "message ", to_string( "%0X", msg.message ), " -> ", get_msg_desc( msg.message ), log::endl;
          #else
          log::input, "message ", to_string( "%0X", msg.message ), log::endl;
          #endif

          TranslateMessage( &msg );

          event.data = &msg;

          if( msg.message == WM_QUIT )

               events::fire( events::exit<>, event );

          else events::fire( events::input_message<>, event );  

        }  

      }

   };



  }
}


