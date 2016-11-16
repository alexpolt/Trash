#pragma once

#include "windows.h"
#include "math/types.h"
#include "os/window.h"


namespace lib {

  namespace os {


    void show_cursor( bool is_cursor ) {

      ShowCursor( is_cursor );
    }

    math::vec2i cursor_xy() {

      POINT pt{};

      GetCursorPos( &pt );

      if( os::window::window_top() ) {

        auto hwnd = os::window::window_top()->handle();

        ScreenToClient( hwnd, &pt );
      }

      return vec2i{ pt.x, pt.y };
    }

    void cursor_center() {

      RECT rc;
      POINT pt;

      auto hwnd = os::window::window_top()->handle();

      GetWindowRect( hwnd, &rc );
      pt.x = ( rc.right - rc.left ) / 2;
      pt.y = ( rc.bottom - rc.top ) / 2;

      ClientToScreen( hwnd, &pt );

      SetCursorPos( pt.x, pt.y );
    }

  }
}


