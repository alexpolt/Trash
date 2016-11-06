#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "event/common.h"
#include "error-win32.h"
#include "input-win32.h"


namespace lib {

  namespace os {


    struct window_win32 : nocopy {
      
      static constexpr cstr window_clsname = "flower_window";
      static constexpr DWORD style_default = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
      static constexpr DWORD style_fullscreen = WS_OVERLAPPED | WS_VISIBLE;

      window_win32( HWND hwnd, cstr title, int w, int h, bool fullscreen ) : 
        _hwnd{ hwnd }, _title{ title }, _w{ w }, _h{ h }, _fs{ fullscreen } { }

      window_win32( window_win32&& other ) : 
        _hwnd{ move( other._hwnd ) }, _title{ other._title }, 
        _w{ other._w }, _h{ other._h }, _fs{ other._fs } { }

      static window_win32 create( cstr title, int width, int height, bool fullscreen = false ) {

        register_class( window_clsname );

        HWND hwnd = CreateWindow(
                      window_clsname, title, ( not fullscreen ? style_default : style_fullscreen ),
                      CW_USEDEFAULT, SW_SHOW, width, height,
                      NULL, NULL, GetModuleHandle( nullptr ), nullptr );

        if( ! hwnd ) $throw $error_window( "create window failed" );

        $event( "input_message" ) {

          DispatchMessage( (MSG*) event.data );

          return true;
        };

        $event( "window" ) {

          if( event.action == action::paint ) 

              ValidateRect( (HWND) event.data, nullptr );

          return true;
        };


        RECT r;

        GetClientRect( hwnd, &r );

        return window_win32{ hwnd, title, r.right, r.bottom, fullscreen };
      }


      static void register_class( cstr clsname ) {

        WNDCLASS cls{ CS_OWNDC | CS_VREDRAW | CS_HREDRAW, wndproc, 0, 0, 
                      GetModuleHandle( nullptr ), NULL, LoadCursor( NULL, IDC_ARROW ), 
                      HBRUSH( COLOR_WINDOW + 1 ), nullptr, clsname };

        auto atom = RegisterClass( &cls );

        if( not atom ) $throw $error_window( "register class failed" );
      }


      static LRESULT CALLBACK wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {

        auto event = event::event_data{};

        event.data = hwnd;

        POINT pt{};

        GetCursorPos( &pt );

        ScreenToClient( hwnd, &pt );

        switch( msg ) {

          case WM_NCCREATE:
            return TRUE;
          break;

          case WM_SIZE:
            event.x = LOWORD( lparam ); 
            event.y = HIWORD( lparam ); 
            if( wparam == 1 ) event.action = action::minimize;
            else if( wparam == 2 ) event.action = action::maximize;
            else event.action = action::resize; 
            log::input, "size, width = ", event.x, ", height = ", event.y;
            log::input, ", action = ", get_action_desc( event.action ), log::endl;
            events::fire( events::window<>, event );
          break;

          case WM_PAINT:
            log::input, "paint", log::endl;
            event.x = pt.x;
            event.y = pt.y;
            event.action = action::paint;
            events::fire( events::window<>, event );
          break;

          case WM_CLOSE:
            log::input, "close", log::endl;
            event.mod = get_modifiers();
            event.action = action::close;
            if( not events::fire( events::window<>, event ) ) DestroyWindow( hwnd );
          break;

          case WM_KEYUP:
          case WM_KEYDOWN: {
            event.key = (vkey) wparam;
            log::input, "key ", get_vkey_desc( event.key ).name, log::endl;
            event.x = pt.x;
            event.y = pt.y;
            event.action = input_map( event.key );
            event.mod = get_modifiers();
            if( msg == WM_KEYUP ) 
                 events::fire( events::key_up<>, event );
            else events::fire( events::key_down<>, event );
          }
          break;

          case WM_LBUTTONUP:
          case WM_LBUTTONDOWN:
          case WM_RBUTTONUP:
          case WM_RBUTTONDOWN:
            event.x = pt.x;
            event.y = pt.y;
            event.mod = get_modifiers();
            if( msg == WM_LBUTTONUP ) { 
              log::input, "mouse left button up", log::endl;
              event.key = vkey::lbutton; 
              event.action = input_map( event.key );
              events::fire( events::mouse_up<>, event );
            } else if( msg == WM_LBUTTONDOWN ) { 
              log::input, "mouse left button down", log::endl;
              event.key = vkey::lbutton; 
              event.action = input_map( event.key );
              events::fire( events::mouse_down<>, event );
            } else if( msg == WM_RBUTTONUP ) { 
              log::input, "mouse rightbutton up", log::endl;
              event.key = vkey::rbutton; 
              event.action = input_map( event.key );
              events::fire( events::mouse_up<>, event );
            } else if( msg == WM_RBUTTONDOWN ) { 
              log::input, "mouse right button down", log::endl;
              event.key = vkey::rbutton; 
              event.action = input_map( event.key );
              events::fire( events::mouse_down<>, event ); 
            }
          break;

          case WM_MOUSEMOVE:
            log::input, "mouse move, x = ", pt.x, ", y = ", pt.y, log::endl;
            event.x = pt.x;
            event.y = pt.y;
            event.key = vkey::null;
            event.mod = get_modifiers();
            event.action = action::move;
            events::fire( events::mouse_move<>, event );
          break;

          case WM_MOUSEWHEEL:
            event.x = 0;
            event.y = short( HIWORD( wparam ) ) / short( WHEEL_DELTA );
            log::input, "scroll delta = ", event.y, log::endl;
            event.mod = get_modifiers();
            event.key = vkey::scroll;
            event.action = input_map( event.key );
            events::fire( events::scroll<>, event );
          break;


          case WM_DESTROY:
            log::input, "destroy", log::endl;
            PostQuitMessage( 0 );
          break;

          default:
            return DefWindowProc( hwnd, msg, wparam, lparam );
        }
        
        return 0;
      }

      ~window_win32() {

        if( IsWindow( _hwnd ) )

          DestroyWindow( _hwnd );
      }


      void close() {

        if( IsWindow( _hwnd ) )

          DestroyWindow( _hwnd );
      }

      auto width() const { return _w; }
      auto height() const { return _h; }
      auto title() const { return _title; }

      HWND _hwnd;
      cstr _title;
      int _w;
      int _h;
      bool _fs;

    };


  }
}



