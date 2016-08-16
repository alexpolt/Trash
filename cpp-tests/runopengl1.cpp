#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

char buf[1024];

int main() {

STARTUPINFO si = {};
PROCESS_INFORMATION pi = {};
char cmd[] = "opengl1.exe";

HANDLE read_h, write_h;
SECURITY_ATTRIBUTES sa = {};
sa.nLength = sizeof sa;
sa.bInheritHandle = TRUE;
sa.lpSecurityDescriptor = NULL;

if(! CreatePipe( &read_h, &write_h, NULL, 0 ) ) {
  printf("error %x\n", GetLastError() );
  return 1;
}
SetHandleInformation( write_h, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT );

si.cb = sizeof si;
si.hStdOutput = write_h;
si.dwFlags = STARTF_USESTDHANDLES;

BOOL result = CreateProcessA( NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi );
if(!result) {
  printf("error %x\n", GetLastError() );
  return 1;
}

CloseHandle( write_h );

DWORD bytes_read;
while( ReadFile( read_h, buf, sizeof buf, &bytes_read, NULL ) ) {
  if( bytes_read == 0 ) break;
  buf[ bytes_read ] = '\0';
  printf("%s", buf );
}

CloseHandle( read_h );
CloseHandle( pi.hThread );
CloseHandle( pi.hProcess );

return 0;
}



