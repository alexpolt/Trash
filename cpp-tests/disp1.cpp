#include <cstdio>
#include <windows.h>



int main() {

DISPLAY_DEVICE dd;
dd.cb = sizeof( dd );
unsigned devid = 0;

printf("Start, devid = %d\n", devid);

while( EnumDisplayDevices( NULL, devid, &dd, 0 ) ) {
  printf( "id = %d, DeviceName = %s, DeviceString = %s\n", devid, dd.DeviceName, dd.DeviceString );
  devid++;
}

if( !devid ) 
  printf("error: %d\n", GetLastError() );

RECT desk = {};
GetWindowRect( GetDesktopWindow(), &desk );
printf("Rect top = %d, left = %d, bottom = %d, right = %d\n", desk.top, desk.left, desk.bottom, desk.right );

DEVMODE dm; 
dm.dmSize = sizeof(dm);
dm.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH | DM_BITSPERPEL;

EnumDisplaySettings( 0, ENUM_CURRENT_SETTINGS, &dm );

//ChangeDisplaySettings( &dm, CDS_FULLSCREEN );

//MSG msg;
//while( GetMessage( &msg, 0, 0, 0 ) ) {
//  printf("Message %d\n", msg.message );
//}

//char buf[16];
//ReadFile( GetStdHandle( STD_INPUT_HANDLE ), buf, sizeof buf, 0, 0 );

//ChangeDisplaySettings( &dm, CDS_RESET );

return 0;
}
