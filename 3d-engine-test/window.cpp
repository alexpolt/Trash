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
      logmacro( logtype::error, windows::get_last_error_string() );
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
      logmacro( logtype::error, windows::get_last_error_string() );
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

    if( ! data_m->hwnd ) log_error_and_terminate {
      logmacro( logtype::error, windows::get_last_error_string() );
      assert( !"CreateWindow failed" );
    }

    if( style & WS_POPUP && style & WS_MAXIMIZE ) fullscreen_begin();

    hwnd_list.insert( { data_m->hwnd, this } );
  }

  void window::destruct( bool wm_destroy ) {
    if( data_m ) {

      if( data_m->style & WS_POPUP && data_m->style & WS_MAXIMIZE ) fullscreen_end();

      if( !wm_destroy ) 
        DestroyWindow( data_m->hwnd );

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

  DEVMODE window::orig_mode{};

  void window::fullscreen_begin() {
    if( orig_mode.dmSize == 0 ) {
      orig_mode.dmSize = sizeof orig_mode;
      orig_mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
      EnumDisplaySettingsA( nullptr, ENUM_CURRENT_SETTINGS, &orig_mode );

      DEVMODE dm = {}; 
      dm.dmPelsWidth = orig_mode.dmPelsWidth;
      dm.dmPelsHeight = orig_mode.dmPelsHeight;
      dm.dmBitsPerPel = orig_mode.dmBitsPerPel;
      dm.dmSize = sizeof(dm);
      dm.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH | DM_BITSPERPEL;
    }

    if( ChangeDisplaySettings( &orig_mode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
      logmacro( logtype::error, windows::get_last_error_string() );
      assert( !"CreateWindow failed" );
    }

  }

  void window::fullscreen_end() {
    if( ChangeDisplaySettings( &orig_mode, CDS_RESET ) != DISP_CHANGE_SUCCESSFUL ) {
      logmacro( logtype::error, windows::get_last_error_string() );
      assert( !"CreateWindow failed" );
    }
  }

  LRESULT window::common_wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {  

    ptrdiff_t n = hwnd_list.find( [hwnd]( hwnd_pair pair ) -> bool { if( pair.hwnd == hwnd ) return true; else return false; } );

    if( n == hwnd_list.notfound() ) {
      if( WM_CREATE == msg ) return 0;
    } else {
      hwnd_pair pair = hwnd_list[ n ];
      return pair.wnd->wndproc( hwnd, msg, wparam, lparam );
    }
    return DefWindowProc( hwnd, msg, wparam, lparam );
  }

  void window::onpaint( onpaintcallback* func ){
    data_m->onpaintcb = func;
  }

  void window::onkeyup( onkeycallback* func ){
    data_m->onkeycb = func;
  }

  LRESULT window::wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {

    switch( msg ) {

      case WM_SIZE:
        data_m->width = LOWORD( lparam );
        data_m->height = HIWORD( lparam );
      break;

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



}}


