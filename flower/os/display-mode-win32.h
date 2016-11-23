#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"
#include "lib/log.h"
#include "error-win32.h"


namespace lib {

  namespace os {


    struct display_mode_win32 {

      display_mode_win32() {

        EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &_dm );

        log::os, $this, log::endl;
      }
      
      void set_fullscreen() {

        auto ret = ChangeDisplaySettings( nullptr, CDS_FULLSCREEN );

        if( ret != DISP_CHANGE_SUCCESSFUL ) 

          $throw $error_win32( " change display settings failed" );

        log::os, $this, " go fullscreen", log::endl;

        _changed = true;
      }

      auto width() const { return _dm.dmPelsWidth; }
      auto height() const { return _dm.dmPelsWidth; }
      auto depth() const { return _dm.dmBitsPerPel; }
      auto frequency() const { return _dm.dmDisplayFrequency; }

      ~display_mode_win32() {

        if( _changed ) {

          log::os, $this, " reset fullscreen", log::endl;

          ChangeDisplaySettings( nullptr, 0 );
        }
      }

      cstr to_string() const {

        return lib::to_string( "display mode ( %dx%d, %d bits, %d hz )", width(), height(), depth(), frequency() );
      }

      bool _changed{ false };
      DEVMODE _dm{};
    };


  }
}


