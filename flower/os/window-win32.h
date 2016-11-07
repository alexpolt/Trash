#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "lib/owner-ptr.h"
#include "lib/scope-guard.h"
#include "event/common.h"
#include "error-win32.h"
#include "input-win32.h"


namespace lib {

  namespace os {


    struct window_win32 : nocopy {
      
      static constexpr cstr window_clsname = "flower_window";
      static constexpr DWORD style_default = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
      static constexpr DWORD style_noresize = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE;
      static constexpr DWORD style_fullscreen = WS_POPUP | WS_VISIBLE;

      struct window_data {
        HWND hwnd;
        cstr title;
        int w;
        int h;
        bool fs;
      };

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

        HWND hwnd = CreateWindow(
                      window_clsname, title, style,
                      CW_USEDEFAULT, SW_SHOW, r.right, r.bottom,
                      NULL, NULL, GetModuleHandle( nullptr ), nullptr );

        if( ! hwnd ) $throw $error_win32( "create window failed" );

        $event( "input_message" ) {

          DispatchMessage( (MSG*) event.data );

          return true;
        };
        auto data = make_owner< window_data >( hwnd, title, width, height, fullscreen );

        SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)data.get() );

        return window_win32{ move( data ) };
      }


      static void register_class( cstr clsname ) {

        WNDCLASS cls{ CS_OWNDC | CS_VREDRAW | CS_HREDRAW, wndproc, 0, $size( void* ), 
                      GetModuleHandle( nullptr ), NULL, LoadCursor( NULL, IDC_ARROW ), 
                      HBRUSH( COLOR_BTNFACE + 1 ), nullptr, clsname };

        auto atom = RegisterClass( &cls );

        if( not atom ) $throw $error_win32( "register class failed" );
      }


      static LRESULT CALLBACK wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {

        auto event = event::event_data{};

        event.data = hwnd;

        POINT pt{};

        GetCursorPos( &pt );

        ScreenToClient( hwnd, &pt );

        auto data = (window_data*) GetWindowLongPtr( hwnd, GWLP_USERDATA );

        switch( msg ) {

          case WM_SIZE:
            event.x = LOWORD( lparam ); 
            event.y = HIWORD( lparam ); 
            if( wparam == 1 ) event.action = action::minimize;
            else if( wparam == 2 ) event.action = action::maximize;
            else event.action = action::resize; 
            log::input, "size, width = ", event.x, ", height = ", event.y, log::endl;
            if( not events::fire( events::window_paint<>, event ) and data ) {
              log::input, "set new size", log::endl;
              data->w = LOWORD( lparam ); 
              data->h = HIWORD( lparam ); 
              return DefWindowProc( hwnd, msg, wparam, lparam );
            }
          break;

          case WM_PAINT:
            log::input, "paint", log::endl;
            event.x = pt.x;
            event.y = pt.y;
            if( not events::fire( events::window_paint<>, event ) ) ValidateRect( hwnd, nullptr );
          break;

          case WM_CLOSE:
            log::input, "close", log::endl;
            event.mod = get_modifiers();
            if( not events::fire( events::window_close<>, event ) ) DestroyWindow( hwnd );
          break;

          case WM_KEYUP:
          case WM_KEYDOWN: {
            event.key = (vkey) wparam;
            log::input, "key ", get_vkey_desc( event.key ), log::endl;
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

        if( $this )

          DestroyWindow( _data->hwnd );
      }


      void close() {

        if( $this )

          DestroyWindow( _data->hwnd );
      }

      void set_data( uint* data ) {

        if( not $this ) $throw $error_win32( "no valid window" );

        HDC dc_orig = GetDC( _data->hwnd );

        $on_return{ ReleaseDC( _data->hwnd, dc_orig ); };

        HDC dc_new = CreateCompatibleDC( dc_orig );
        
        $on_return{ DeleteDC( dc_new ); };

        HBITMAP bitmap_new = CreateCompatibleBitmap( dc_orig, width(), height() );

        HBITMAP bitmap_old = (HBITMAP) SelectObject( dc_new, bitmap_new );

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

      owner_ptr< window_data > _data;
    };


  }
}



