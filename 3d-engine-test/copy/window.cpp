#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "config.h"
#include "windows.h"
#include "window.h"
#include "result.h"
#include "heap.h"
#include "util.h"
#include "usertypes/fixedarray.h"


namespace alexpolt { namespace ui {

  /*
    Window object. Creation and management.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  fixed_array<hwnd_pair, MAX_HWND> hwnd_list;

  void register_window_class();
  void unregister_window_class();

  void begin() {
    register_window_class();
  }

  void end() {
    unregister_window_class();
  }

  void unregister_window_class() {
    BOOL r = UnregisterClass( config::appname.data(), windows::gethInstance() ) ;
    if( !r ) { 
      logging::log( logging::logtype::error, windows::getLastErrorString() );
      assert( !"UnregisterClass failed" );
    }
  }

    
  void register_window_class() {
    static WNDCLASS wndclass;

    wndclass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    wndclass.lpfnWndProc = window::common_wndproc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = windows::gethInstance();
    wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
    wndclass.hbrBackground = (HBRUSH)( 1 + COLOR_BTNFACE );
    wndclass.lpszMenuName = nullptr;
    wndclass.lpszClassName = config::appname.data();

    if( RegisterClass( &wndclass ) == 0 ) {
      logging::log( logging::logtype::error, windows::getLastErrorString() );
      assert( !"RegisterClass failed" );
    }
  }
  

  void window::construct( cstring title, unsigned width, unsigned height, HWND parent, DWORD style ) {
    using datatype = util::remove_reference< decltype(*data_m) >::type;

    data_m = heap::malloc< datatype >();

    data_m->parent = parent; data_m->title = title; data_m->width = width; data_m->height = height; data_m->style = style; 
    data_m->onpaintcb = nullptr;
    data_m->onkeycb = nullptr;

    data_m->hwnd = CreateWindowA( config::appname.data(), title.data(), style, CW_USEDEFAULT, CW_USEDEFAULT, width, height, parent, NULL, windows::gethInstance(), 0 );

    if( ! data_m->hwnd ) {
      logging::log( logging::logtype::error, windows::getLastErrorString() );
      assert( !"CreateWindow failed" );
    }

    hwnd_list.insert( { data_m->hwnd, this } );
  }

  void window::destruct( bool wm_destroy ) {
    if( data_m && !wm_destroy ) 
       DestroyWindow( data_m->hwnd );

    if( data_m ) {
      HWND hwnd = data_m->hwnd;
      size_t n = hwnd_list.find( [hwnd]( hwnd_pair pair ) -> bool { if( pair.hwnd == hwnd ) return true; else return false; } );
      hwnd_list.erase( n );
      heap::free( data_m );
    } 
    data_m = nullptr;
  }

  window::window( window&& r ) : data_m( r.data_m ) {
     HWND hwnd = r.data_m->hwnd;
     ptrdiff_t n = hwnd_list.find( [hwnd]( hwnd_pair pair ) -> bool { if( pair.hwnd == hwnd ) return true; else return false; } );
     if( n == hwnd_list.notfound() ) assert( !"HWND not found while moving" );
     hwnd_list.insert( n, { hwnd, this } ); //update pointer
     r.data_m = nullptr;
  }

  window& window::operator=( window&& r ) {
    if( this == &r ) assert( !"Self-assigment" );
    heap::free( data_m );
    data_m = r.data_m;
    r.data_m = nullptr;
    return *this;
  }

  void window::show( INT cmd ) const {
    ShowWindow( data_m->hwnd, cmd );
  }

  HDC window::dc() const { return GetDC( hwnd() ); }
  HWND window::hwnd() const { return data_m->hwnd; }

  LRESULT window::common_wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {  

    ptrdiff_t n = hwnd_list.find( [hwnd]( hwnd_pair pair ) -> bool { if( pair.hwnd == hwnd ) return true; else return false; } );

    if( n == hwnd_list.notfound() ) {
      if( WM_CREATE == msg ) return 0;
      if( WM_NCCREATE == msg ) return 1;
      if( WM_GETMINMAXINFO == msg ) return 0;
    } else {
      hwnd_pair pair = hwnd_list[ n ];
      return pair.wnd->wndproc( hwnd, msg, wparam, lparam );
    }
    return DefWindowProc( hwnd, msg, wparam, lparam );
  }


  LRESULT window::wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {

    switch( msg ) {

      case WM_SIZING:
        RECT* rect;
        rect = (RECT*)lparam;
        data_m->width = rect->right - rect->left;
        data_m->height = rect->bottom - rect->top;
        return 1;
      break;

      case WM_KEYUP:
          if( data_m->onkeycb )
            data_m->onkeycb->key( *this, wparam );
      break;

      case WM_KEYDOWN:
        if( wparam == VK_ESCAPE ) {
          int result = MessageBoxA( hwnd, "Are you sure?"_str, config::appname, MB_ICONQUESTION | MB_YESNOCANCEL | MB_DEFBUTTON2 );
          if( result == IDYES ) 
            DestroyWindow( hwnd );
      }
      break;

      case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc;
        InvalidateRect( hwnd, NULL, FALSE );
        hdc = BeginPaint( hwnd, &ps );

        if( data_m->onpaintcb )
          data_m->onpaintcb->paint( *this, hdc );

        EndPaint( hwnd, &ps );
        break;

      case WM_DESTROY: 
        this->destruct( true );
        PostQuitMessage(0); 
        break;

      default:
        return DefWindowProc( hwnd, msg, wparam, lparam );
    }

    return 0;
  }

  void window::onpaint( onpaintcallback* func ){
    data_m->onpaintcb = func;
  }

  void window::onkeyup( onkeycallback* func ){
    data_m->onkeycb = func;
  }


}}


