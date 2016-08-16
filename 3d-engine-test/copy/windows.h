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


  inline HMODULE gethInstance() {
    return GetModuleHandle( NULL );
  }

  inline string getLastErrorString() {
    DWORD lasterror = GetLastError();
    DWORD numchars;
    numchars = FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                                          gethInstance( ), lasterror, (DWORD)NULL, global::tempbuffer.data(), global::tempbuffer.size(), (DWORD)NULL );
    printf("%s: numchars = %d, string = %s\n", __func__, (int)numchars, global::tempbuffer.data() );
    return string{ global::tempbuffer };
  }



}}


#endif //alexpolt_windows

