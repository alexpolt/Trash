#include <windows.h>
#include <cstdio>
#include <cstring>

BOOL CALLBACK enumwindows(HWND hwnd, LONG lparam) {
  static char buffer[256];

  GetWindowText( hwnd, buffer, sizeof buffer );
  HWND hwnd_p = GetParent(hwnd);

  if( lparam == 0 ) {
    if( buffer[1] == ':' && buffer[0] && ! hwnd_p ) printf("%s\n", buffer);
  } else {
    if( strstr( buffer, "Sumatra" ) ) printf("%s\n", buffer);
  }

  return TRUE;
}

int main() {
  EnumWindows( enumwindows, 0 );
  printf( "\n" );
  EnumWindows( enumwindows, 1 );
}

