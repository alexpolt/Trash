#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/assert.h"
#include "lib/types.h"
#include "lib/log.h"
#include "lib/to-string.h"
#include "lib/owner-ptr.h"
#include "lib/scope-guard.h"
#include "types.h"
#include "event/common.h"
#include "events.h"
#include "error-win32.h"
#include "vkey.h"
#include "vkey-desc.h"
#include "action.h"
#include "input-map-win32.h"


namespace lib {

  namespace os {


    vmod get_modifiers();

    struct window_win32;

    struct window_data {
      os::hwnd hwnd;
      cstr title;
      int w;
      int h;
      bool fs;
      event::eid_t eid;
      window_data* data_prev;
    };

    namespace global {

      TP<TN...>
      window_data* window_top_data;
    }


    struct window_win32 : nocopy {
      
      static constexpr cstr window_clsname = "flower_engine_window";

      static constexpr DWORD style_default = 
        WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;

      static constexpr DWORD style_noresize = 
        WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

      static constexpr DWORD style_fullscreen = 
        WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP;


      enum class cmd : int { show, hide, };


      window_win32( window_data* data, bool is_copy ) : _data{ data }, _is_copy{ is_copy } { }

      window_win32( owner_ptr< window_data > data ) : _data{ move( data ) } { }

      window_win32( window_win32&& other ) : _data{ move( other._data ) }  { }


      static window_win32 create( cstr title, int width, int height, 
                                  bool resize = false, bool fullscreen = false ) {

        register_class( window_clsname );

        auto style = style_default;
        
        if( not resize ) style = style_noresize;
        if( fullscreen ) style = style_fullscreen;

        RECT r{ 0, 0, width, height };

        AdjustWindowRect( &r, style, false );

        r.right = r.right - r.left;
        r.bottom = r.bottom - r.top;

        os::hwnd hwnd = CreateWindow(
                          window_clsname, title, style,
                          CW_USEDEFAULT, CW_USEDEFAULT, r.right, r.bottom,
                          NULL, NULL, GetModuleHandle( nullptr ), nullptr );

        if( ! hwnd ) $throw $error_win32( "create window failed" );

        auto data = make_owner< window_data >( hwnd, title, width, height, fullscreen );

        data->eid = $event( "input_message", "window dispatch" ) {

          DispatchMessage( (MSG*) event.data );

          return true;
        };

        SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)data.get() );

        data->data_prev = global::window_top_data<>;

        global::window_top_data<> = data.get();

        auto w = window_win32{ move( data ) };
        
        log::os, w, " created", log::endl;

        return w;
      }


      static void register_class( cstr clsname ) {

        WNDCLASS cls{ CS_OWNDC | CS_VREDRAW | CS_HREDRAW, wndproc, 0, $size( void* ), 
                      GetModuleHandle( nullptr ), NULL, LoadCursor( NULL, IDC_ARROW ), 
                      HBRUSH( COLOR_BTNFACE + 1 ), nullptr, clsname };

        RegisterClass( &cls );
      }


      static LRESULT CALLBACK wndproc( os::hwnd hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {

        auto event = event::event_data{};

        event.data = hwnd;

        auto data_ptr = (window_data*) GetWindowLongPtr( hwnd, GWLP_USERDATA );

        if( data_ptr ) event.data = data_ptr;

        switch( msg ) {

          case WM_SIZE:
            event.x = LOWORD( lparam ); 
            event.y = HIWORD( lparam ); 
            if( wparam == 1 ) event.action = action::minimize;
            else if( wparam == 2 ) event.action = action::maximize;
            else event.action = action::resize; 
            log::input, "size, width = ", event.x, ", height = ", event.y, log::endl;
            if( data_ptr ) {
              log::input, "setting new window size", log::endl;
              data_ptr->w = LOWORD( lparam );
              data_ptr->h = HIWORD( lparam );
            }
            if( events::fire( events::window_resize<>, event ) ) return 0;
         break;

          case WM_PAINT:
            log::input, "paint", log::endl;
            if( events::fire( events::window_paint<>, event ) ) return 0;
          break;

          case WM_CLOSE:
            log::input, "close", log::endl;
            event.mod = get_modifiers();
            if( events::fire( events::window_close<>, event ) ) { 
              DestroyWindow( hwnd );
              return 0;
            }
          break;

          case WM_SYSKEYUP:
          case WM_SYSKEYDOWN:
          case WM_KEYUP:
          case WM_KEYDOWN:
            event.key = (vkey) wparam;
            log::input, "key ", get_vkey_desc( event.key ), log::endl;
            event.action = input_map( event.key );
            event.mod = get_modifiers();
            if( msg == WM_KEYUP or msg == WM_SYSKEYUP ) {
                   if( events::fire( events::key_up<>, event ) ) return 0;
            } else if( events::fire( events::key_down<>, event ) ) return 0;
          break;

          case WM_LBUTTONUP:
          case WM_LBUTTONDOWN:
          case WM_RBUTTONUP:
          case WM_RBUTTONDOWN:
            event.x = LOWORD( lparam ); 
            event.y = HIWORD( lparam ); 
            event.mod = get_modifiers();
            if( msg == WM_LBUTTONUP ) { 
              log::input, "mouse left button up", log::endl;
              event.key = vkey::lbutton; 
              event.action = input_map( event.key );
              if( events::fire( events::mouse_up<>, event ) ) return 0;
            } else if( msg == WM_LBUTTONDOWN ) { 
              log::input, "mouse left button down", log::endl;
              event.key = vkey::lbutton; 
              event.action = input_map( event.key );
              if( events::fire( events::mouse_down<>, event ) ) return 0;
            } else if( msg == WM_RBUTTONUP ) { 
              log::input, "mouse rightbutton up", log::endl;
              event.key = vkey::rbutton; 
              event.action = input_map( event.key );
              if( events::fire( events::mouse_up<>, event ) ) return 0;
            } else if( msg == WM_RBUTTONDOWN ) { 
              log::input, "mouse right button down", log::endl;
              event.key = vkey::rbutton; 
              event.action = input_map( event.key );
              if( events::fire( events::mouse_down<>, event ) ) return 0;
            }
          break;

          case WM_MOUSEMOVE:
            event.x = LOWORD( lparam ); 
            event.y = HIWORD( lparam ); 
            log::input, "mouse move, x = ", event.x, ", y = ", event.y, log::endl;
            event.key = vkey::null;
            event.mod = get_modifiers();
            event.action = action::move;
            if( events::fire( events::mouse_move<>, event ) ) return 0;
          break;

          case WM_MOUSEWHEEL:
            event.x = 0;
            event.y = short( HIWORD( wparam ) ) / short( WHEEL_DELTA );
            log::input, "scroll delta = ", event.y, log::endl;
            event.mod = get_modifiers();
            event.key = vkey::scroll;
            event.action = input_map( event.key );
            if( events::fire( events::scroll<>, event ) ) return 0;
          break;

          case WM_ACTIVATEAPP:
            log::input, "activate ", (bool) wparam, log::endl;
            event.x = wparam;
            event.action = action::activate;
            if( events::fire( events::window_activate<>, event ) ) return 0;
          break;

          case WM_SYSCOMMAND:
            if( wparam == SC_MONITORPOWER ) {
              event.action = lparam == -1 ? action::on : 
                                ( lparam == 1 ? action::lowpower : action::off );
              log::input, "monitorpower ", get_action_desc( event.action ), log::endl;
              if( events::monitor<>->size() )
                return events::fire( events::monitor<>, event );
              else break;
            } else if( wparam == SC_SCREENSAVE ) {
              log::input, "screensaver", log::endl;
              if( events::screensaver<>->size() )
                return events::fire( events::screensaver<>, event );
              else break;
             }
          break;

          case WM_INPUT: {
            UINT dwSize = 40;
            static BYTE lpb[40];
        
            GetRawInputData((HRAWINPUT)lparam, RID_INPUT, 
                            lpb, &dwSize, sizeof(RAWINPUTHEADER));
        
            RAWINPUT* raw = (RAWINPUT*)lpb;
        
            if (raw->header.dwType == RIM_TYPEMOUSE) {
              event.x = raw->data.mouse.lLastX;
              event.y = raw->data.mouse.lLastY;
              event.key = vkey::null;
              event.mod = get_modifiers();
              event.action = action::move;
              events::fire( events::mouse_rinput<>, event );
              return 0;
            }
          }
          break;          

          case WM_DESTROY:
            log::input, "destroy", log::endl;
            events::fire( events::window_destroy<>, event );
            return 0;
        }

        return DefWindowProc( hwnd, msg, wparam, lparam );
      }

      ~window_win32() { 

        if( _is_copy ) _data = nullptr;
          
        destroy(); 
      }

      void destroy() {

        $assert( (bool)$this, "for some reason window handle is not valid" );

        log::os, $this, " destroying", log::endl;

        DestroyWindow( _data->hwnd );

        event::remove( "input_message", _data->eid );

        global::window_top_data<> = _data->data_prev;
      }

      static int cmd_map( cmd c ) {
        static int map[]{ SW_SHOW,  SW_HIDE };
        return map[ (int) c ];
      }

      static cstr get_cmd_desc( cmd c ) {
        static cstr cmd_desc[]{ "show", "hide" };
        return cmd_desc[ (int) c ];
      }

      void show( cmd c = cmd::show ) {

        log::os, $this, " ", get_cmd_desc( c ), log::endl;

        ShowWindow( _data->hwnd, cmd_map( c ) );
      }

      void set_data( uint* data ) {

        if( not $this ) $throw $error_win32( "no valid window" );

        auto dc_orig = GetDC( _data->hwnd );

        $on_return{ ReleaseDC( _data->hwnd, dc_orig ); };

        auto dc_new = CreateCompatibleDC( dc_orig );
        
        $on_return{ DeleteDC( dc_new ); };

        auto bitmap_new = CreateCompatibleBitmap( dc_orig, width(), height() );

        auto bitmap_old = (HBITMAP) SelectObject( dc_new, bitmap_new );

        $on_return{ SelectObject( dc_new, bitmap_old ); DeleteObject( bitmap_new ); };

        BITMAPINFOHEADER binfo{};

        binfo.biSize = $size ( binfo );
        binfo.biWidth = width();
        binfo.biHeight = -height();
        binfo.biPlanes = 1;
        binfo.biBitCount = 32;
        binfo.biCompression = BI_RGB;

        auto r = SetDIBits( dc_new, bitmap_new, 0, height(), data, 
                            (BITMAPINFO*) &binfo, DIB_RGB_COLORS );

        if( not r ) $throw $error_win32( "SetDIBits failed" );

        BitBlt( dc_orig, 0, 0, width(), height(), dc_new, 0, 0, SRCCOPY );
      }

      explicit operator bool() const { return IsWindow( _data->hwnd ); }

      int width() const { return _data->w; }
      int height() const { return _data->h; }
      cstr title() const { return _data->title; }
      os::hwnd handle() const { return _data->hwnd; }

      cstr to_string() const { 

        return lib::to_string( "window %s( %dx%d, %p )", _data->title, _data->w, _data->h, (void*) _data->hwnd ); 
      }

      static auto window_top() { return window_win32{ global::window_top_data<>, true }; }

      owner_ptr< window_data > _data;
      bool _is_copy{ false };
    };

    inline vmod get_modifiers() {

      vmod mod = vmod::null;

      const short mask = 0x8000;

      if( GetKeyState( (int) vkey::shift ) & mask ) mod = vmod( mod | vmod::shift );
      if( GetKeyState( (int) vkey::control ) & mask ) mod = vmod( mod | vmod::control );
      if( GetKeyState( (int) vkey::menu ) & mask ) mod = vmod( mod | vmod::menu );

      return mod;
    }


  }
}



