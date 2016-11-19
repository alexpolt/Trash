#pragma once

#include "windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "lib/time.h"


namespace lib {

  namespace os {


    struct timer_win32 {

      timer_win32() {

        QueryPerformanceCounter( &_start );
      }

      time operator()() {

        QueryPerformanceCounter( &_end );

        LARGE_INTEGER freq{};

        QueryPerformanceFrequency( &freq );

        double delta = _end.QuadPart - _start.QuadPart;

        return time( delta / freq.QuadPart );
      }

      LARGE_INTEGER _start{};
      LARGE_INTEGER _end{};
    };


  }
}


