#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

int main() {

WSADATA wsa;
struct clean { ~clean(){printf("cleanup\n"); WSACleanup();} } clean;

printf("init\n");
int r = WSAStartup( MAKEWORD(2,2), &wsa );
if( 0 != r ) { printf("error init %d\n", r); return 1; }

SOCKET sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
if( INVALID_SOCKET == sock ) { printf("no socket %d\n",WSAGetLastError()); return 1; }

sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_addr.s_addr =  htonl(INADDR_ANY);
addr.sin_port = htons(55229);

r = bind( sock, (sockaddr*) &addr, sizeof (addr) );
if( SOCKET_ERROR == r ) { printf("error bind %d\n", WSAGetLastError()); return 1; }

char buf[32];
r = recv( sock, buf, sizeof buf, 0 );
if( SOCKET_ERROR == r ) printf("socket error %d\n", WSAGetLastError());
printf("got %d bytes, string: %.*s", r, r, buf);


}

