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

        auto delta = _end.QuadPart - _start.QuadPart;

        auto seconds = delta / freq.QuadPart;

        delta -= seconds;

        uint mseconds = delta * 1000 / freq.QuadPart;

        mseconds = mseconds % 1000;

        return time{ (uint) seconds, mseconds };
      }

      LARGE_INTEGER _start{};
      LARGE_INTEGER _end{};
    };


  }
}


