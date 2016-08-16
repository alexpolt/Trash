#ifndef alexpolt_ui
#  define alexpolt_ui

#include "windows.h"
#include "config.h"
#include "common.h"
#include "types/object.h"
#include "usertypes/fixedarray.h"

namespace alexpolt { namespace ui {

  /*
    Window object. Creation and management.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  void begin();
  void end(); 

  const int MAX_HWND = 10;

  struct window;

  struct hwnd_pair {
    HWND hwnd;
    window* wnd;
  };

  extern fixed_array<hwnd_pair, MAX_HWND> hwnd_list;

  struct onpaintcallback {
    virtual void paint( const window&, HDC ) = 0;
  };
  struct onkeycallback {
    virtual void key( const window&, WPARAM ) = 0;
  };

  struct window_params {
    HWND hwnd, parent;
    DWORD style;
    unsigned width, height;
    cstring title;
    onpaintcallback* onpaintcb;
    onkeycallback* onkeycb;
  };

  struct window : object {
    static const int defwindowsize = 400;

    window_params* data_m; //no smart pointer, not necessary
    static DEVMODE orig_mode;

    window() {}
    ~window() { destruct(); }
    delete_copy_assign( window )

    window( window&& r );
    window& operator=( window&& r );

    void construct( cstring title, unsigned width, unsigned height, HWND parent, DWORD style = WS_OVERLAPPED );
    void destruct( bool wm_destroy = false );

    static LRESULT CALLBACK common_wndproc( HWND, UINT, WPARAM, LPARAM );
    LRESULT CALLBACK wndproc( HWND, UINT, WPARAM, LPARAM );

    void show( INT cmd = SW_SHOW ) const;

    HDC dc() const { return GetDC( hwnd() ); }
    HWND hwnd() const { return data_m->hwnd; }
    unsigned width() const { return data_m->width; }
    unsigned height() const { return data_m->height; }

    void fullscreen_begin();
    void fullscreen_end();

    void onpaint( onpaintcallback* );
    void onkeyup( onkeycallback* );

  };


  inline result<window> make_default_window( cstring title = config::appname, unsigned width = window::defwindowsize, unsigned height = window::defwindowsize, 
                                                DWORD style = WS_OVERLAPPEDWINDOW ) {
    return make_object<window>( title, width, height, (HWND)NULL, style );
  }

  inline result<window> make_popup_window( cstring title = config::appname, unsigned width = window::defwindowsize, unsigned height = window::defwindowsize ) {
    return make_default_window( title, width, height, WS_POPUP );
  }

  inline result<window> make_fullscreen_window( ) {
    DEVMODE orig_mode{};
    orig_mode.dmSize = sizeof orig_mode;
    orig_mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
    EnumDisplaySettingsA( nullptr, ENUM_CURRENT_SETTINGS, &orig_mode );
    return make_default_window( ""_str, orig_mode.dmPelsWidth, orig_mode.dmPelsHeight, WS_POPUP | WS_MAXIMIZE );
  }


}

  using ui::window;
  using ui::make_default_window;
  using ui::make_popup_window;
  using ui::make_fullscreen_window;
}

#endif //alexpolt_window

