#define WIN32_MEAN_AND_LEAN
#include <windows.h>
#include "opengl1rc.h"
#include <cstdio>
#include "raii.cpp"
#include "opengl1-funcs.cpp"

void process_ok( char* buf ) {
  printf("%s\n", buf );
}

INT_PTR CALLBACK wndProc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
  switch( msg ) {
    case WM_INITDIALOG:
      return TRUE;
    case WM_CLOSE:
      printf("Quitting\n");
      PostQuitMessage( 0 );
      break;
    case WM_COMMAND:
      if( LOWORD(wparam) == IDOK ) {
        char buf1[512], buf2[512];
        GetDlgItemTextA( hwnd, IDC_EDIT1, buf1, sizeof buf1 );
        GetDlgItemTextA( hwnd, IDC_EDIT2, buf2, sizeof buf2 );
        opengl_delshader();
        opengl_shader( buf1, buf2 );
      }
      if( LOWORD(wparam) == IDB_OPENGL ) {
        process_opengl( GetDlgItem( hwnd, IDC_OPENGL ) );
      }
  }
  return 0;
}

void print_last_error() {
  static char buf[256];
  FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, buf, sizeof buf, NULL );
  printf("Error 0x%x, Message: %s\n", (unsigned int) GetLastError(), buf );
}

int CALLBACK WinMain( HINSTANCE hinst, HINSTANCE hinstprev, LPSTR cmdline, int ncmdshow) {

printf("Start app\n");

auto dialog1_h = raii( CreateDialog( hinst, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, wndProc ) , [](HWND hwnd) { DestroyWindow( hwnd ); } );

if(! dialog1_h ) { print_last_error(); return 1; }

HWND opengl_h = GetDlgItem( dialog1_h, IDC_OPENGL );
bool ret = opengl_begin( opengl_h );
if(!ret)  { print_last_error(); return 1; }

SetDlgItemTextA( dialog1_h, IDC_EDIT1, vertexshader );
SetDlgItemTextA( dialog1_h, IDC_EDIT2, fragmentshader );

MSG msg;
while( int res = GetMessage( &msg, NULL, 0, 0 ) ){
  if( res == -1 ) { print_last_error(); break; }
  if(! IsDialogMessage( dialog1_h, &msg ) ) {
    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }
  fflush( stdout );
}

opengl_end();

return 0;
}

