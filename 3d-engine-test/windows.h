#ifndef alexpolt_windows
#  define alexpolt_windows

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "config.h"
#include "usertypes/string.h"

namespace alexpolt { namespace windows {

  /*
    Windows specific.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */


  inline HMODULE get_hinstance() {
    return GetModuleHandle( NULL );
  }

  inline string get_last_error_string() {
    DWORD lasterror = GetLastError();
    DWORD numchars;
    numchars = FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                                          gethInstance( ), lasterror, (DWORD)NULL, global::tempbuffer.data(), global::tempbuffer.size(), (DWORD)NULL );
    printf("%s: numchars = %d, string = %s\n", __func__, (int)numchars, global::tempbuffer.data() );
    return string{ global::tempbuffer };
  }

  inline void log_error_and_terminate( string&& error ) {
    log( error );
    assert( !"Windows error" );
  }


}}


#endif //alexpolt_windows

