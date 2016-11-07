#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "error-win32.h"


namespace lib {

  namespace os {


    struct display_mode_win32 {

      display_mode_win32() {

        EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &_dm );
      }
      
      void set_fullscreen() {

        auto ret = ChangeDisplaySettings( nullptr, CDS_FULLSCREEN );

        if( ret != DISP_CHANGE_SUCCESSFUL ) 

          $throw $error_win32( " change display settings failed" );
      }

      auto width() const { return _dm.dmPelsWidth; }
      auto height() const { return _dm.dmPelsWidth; }
      auto depth() const { return _dm.dmBitsPerPel; }
      auto frequency() const { return _dm.dmDisplayFrequency; }

      ~display_mode_win32() {

        if( _changed ) {

          ChangeDisplaySettings( nullptr, 0 );
        }
      }

      bool _changed{ false };
      DEVMODE _dm{};
    };


  }
}


