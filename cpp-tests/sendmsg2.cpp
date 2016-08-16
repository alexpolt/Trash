#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <cstdio>

#pragma comment(lib,"ws2_32.lib")

int main() {
  HWND wnd = FindWindow(nullptr, "B:\\");
  if(!wnd) {
    printf("Window not found\n");
    getchar();
    return 0;
  }

/*
  int code = 3 << 30 | MapVirtualKey( VK_F5, MAPVK_VK_TO_VSC ) << 16 | 1;
  int f5char = MapVirtualKey( VK_F5, MAPVK_VK_TO_CHAR );
  while(1) {
    HWND curwnd = GetForegroundWindow();
    SendMessage( wnd, WM_KEYDOWN, VK_F5, 1 );
    SendMessage( wnd, WM_KEYUP, VK_F5,  1 );
    SendMessage( wnd, WM_CHAR, f5char,  1 );
    SendMessage( wnd, WM_COMMAND, 1<<16|41504, 0 );
    Sleep(100);
    UpdateWindow( wnd );
  };
  getchar();
  return 0;  
*/

  WSADATA wsd;
  struct wsacleanup { ~wsacleanup() { WSACleanup(); } } wsacleanup;
  WSAStartup( MAKEWORD(2,2), &wsd );

  SOCKET sock = socket( AF_INET, SOCK_DGRAM, 0 );

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1906);
  unsigned long sin_addr = inet_addr( "10.10.3.2" );
  addr.sin_addr = *(in_addr*)&sin_addr;

  int r = bind( sock, (sockaddr*)&addr, sizeof addr );
  if( SOCKET_ERROR == r ) { printf("bind error %d\n", WSAGetLastError() ); return 0; }

  while(true) {
    char msg[32] = {};
    int r = recv( sock, msg, sizeof msg, 0 );
  
    if( r > 0 ) {
      msg[r] = '\0';
      printf( "got message %s\n", msg );
      SendMessage( wnd, WM_COMMAND, 1<<16|41504, 0 );
      //MessageBox( NULL, msg, "WARNING!", MB_OK|MB_ICONEXCLAMATION|MB_SYSTEMMODAL|MB_SETFOREGROUND );
    }
  }

  closesocket( sock );
}

