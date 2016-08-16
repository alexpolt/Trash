#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <cstdio>

#pragma comment(lib,"ws2_32.lib")

int main() {
  WSADATA wsd;
  struct wsacleanup { ~wsacleanup() { WSACleanup(); } } wsacleanup;

  WSAStartup( MAKEWORD(2, 2), &wsd );

  SOCKET sock = socket( AF_INET, SOCK_DGRAM, 0 );
  if( sock == INVALID_SOCKET ) { printf("sock error\n"); return 0; }

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1906);
  unsigned long sin_addr = inet_addr( "10.10.3.2" );
  addr.sin_addr = *(in_addr*)&sin_addr;

  const char msg[] = "Detected!";

  int r = sendto( sock, msg, sizeof msg, 0, (sockaddr*)&addr, sizeof addr );
  if( r == SOCKET_ERROR ) { printf("send error %d\n", WSAGetLastError() ); return 0; }


  closesocket( sock );
}

