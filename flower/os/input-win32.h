#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "lib/to-string.h"
#include "event/common.h"
#include "input-map-win32.h"
#include "types.h"
#include "vkey.h"
#include "action.h"
#include "vkey-desc.h"
#ifdef $CONFIG_DEVMODE
  #include "msg-desc.h"
#endif


namespace lib {

  namespace os {


    struct input_win32 {

      using events_it = event::events<>::iterator;
      
      input_win32() :      
        key_up{ event::create( "key_up" ) },
        key_down{ event::create( "key_down" ) },
        mouse_up{ event::create( "mouse_up" ) },
        mouse_down{ event::create( "mouse_down" ) },
        mouse_move{ event::create( "mouse_move" ) },
        scroll_up{ event::create( "scroll_up" ) },
        scroll_down{ event::create( "scroll_down" ) },
        window{ event::create( "window" ) },
        input_message{ event::create( "input_message" ) },
        exit{ event::create( "exit" ) }
        { }

      void operator()() {

        MSG msg;

        event = event::event_data{};

        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
          
          #ifdef $CONFIG_DEVMODE
          log::input, "message ", get_msg_desc( msg.message ), endl;
          #else
          log::input, "message ", to_string( "%#0X", msg.message ), endl;
          #endif

          event.data = &msg;

          if( not fire( input_message ) ) return;

          TranslateMessage( &msg );

          if( msg.hwnd ) ScreenToClient( msg.hwnd, &msg.pt );

          switch( msg.message ) {

            case WM_QUIT:
              event.x = msg.pt.x;
              event.y = msg.pt.y;
              fire( exit );
            break;

            case WM_SIZE:
              event.x = LOWORD( msg.lParam ); 
              event.y = HIWORD( msg.lParam ); 
              if( msg.wParam == 1 ) event.action = action::minimize;
              else if( msg.wParam == 2 ) event.action = action::maximize;
              else event.action = action::resize; 
              fire( window );
            break;

            case WM_PAINT:
              event.x = msg.pt.x;
              event.y = msg.pt.y;
              event.action = action::paint;
              fire( window );
            break;

            case WM_CLOSE:
              event.x = msg.pt.x;
              event.y = msg.pt.y;
              event.action = action::close;
              fire( window );
            break;

            case WM_KEYUP:
            case WM_KEYDOWN: {
              event.key = (vkey) msg.wParam;
              log::input, "key ", get_vkey_desc( event.key ).name, endl;
              event.x = msg.pt.x;
              event.y = msg.pt.y;
              event.action = input_map( event.key );
              event.mod = get_modifiers();
              if( msg.message == WM_KEYUP ) fire( key_up );
              else fire( key_down );
            }
            break;

            case WM_LBUTTONUP:
            case WM_LBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_RBUTTONDOWN:
              log::input, "mouse button up/down", endl;
              event.x = msg.pt.x;
              event.y = msg.pt.y;
              event.mod = get_modifiers();
              if( msg.message == WM_LBUTTONUP ) { 
                event.key = vkey::lbutton; 
                event.action = input_map( event.key );
                fire( mouse_up );
              } else if( msg.message == WM_LBUTTONDOWN ) { 
                event.key = vkey::lbutton; 
                event.action = input_map( event.key );
                fire( mouse_down );
              } else if( msg.message == WM_RBUTTONUP ) { 
                event.key = vkey::rbutton; 
                event.action = input_map( event.key );
                fire( mouse_up );
              } else if( msg.message == WM_RBUTTONDOWN ) { 
                event.key = vkey::rbutton; 
                event.action = input_map( event.key );
                fire( mouse_down ); 
              }
            break;

            case WM_MOUSEMOVE:
              log::input, "mouse move, x = ", msg.pt.x, ", y = ", msg.pt.y, endl;
              event.x = msg.pt.x;
              event.y = msg.pt.y;
              event.key = vkey::null;
              event.mod = get_modifiers();
              event.action = action::move;
              fire( mouse_move );
            break;

            case WM_MOUSEWHEEL:
              short scroll = short( HIWORD( msg.wParam ) ) / short( WHEEL_DELTA );
              log::input, "scroll delta = ", scroll, endl;
              event.x = 0;
              event.y = scroll;
              event.mod = get_modifiers();
              event.key = vkey::scroll;
              if( scroll > 0 ) {
                event.action = input_map( vkey::next );
                fire( scroll_up );
              } else {
                event.action = input_map( vkey::prior );
                fire( scroll_down );
              }
            break;

          }

       }
        
      }

      vmod get_modifiers() {

        vmod mod = vmod::null;

        const short mask = 0x8000;

        if( GetKeyState( (int) vkey::shift ) & mask ) mod = vmod( mod | vmod::shift );
        if( GetKeyState( (int) vkey::control ) & mask ) mod = vmod( mod | vmod::control );
        if( GetKeyState( (int) vkey::menu ) & mask ) mod = vmod( mod | vmod::menu );

        return mod;
      }

      bool fire( events_it& it ) {

        for( auto& e : *it ) if( not e( event ) ) return false;

        return true;
      }


      event::event_data event;
      events_it key_up;
      events_it key_down;
      events_it mouse_up;
      events_it mouse_down;
      events_it mouse_move;
      events_it scroll_up;
      events_it scroll_down;
      events_it window;
      events_it input_message;
      events_it exit;
    };


  }
}


