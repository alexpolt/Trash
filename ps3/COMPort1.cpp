// COMPort1.cpp : Defines the entry point for the console application.
//

//#define _UNICODE
#include "stdafx.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void readData();
int waitOk(int,int);
int readEcho();
void programData();

HANDLE com1_h, console_h;
char buf[1<<12] = {0};
DWORD read;

char h2b[256] = {0};
#define H2B(hex) h2b[hex]

int main(int argc, char* argv[])
{

	printf("Hello World!\n");
	int a;
	for(a='a';a < 'f'+1; a++) h2b[a]=a-'a'; 
	for(a='A';a < 'F'+1; a++) h2b[a]=a-'A'; 
	for(a='0';a < '9'+1; a++) h2b[a]=a-'0';

	com1_h = CreateFile("com1", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING|FILE_FLAG_WRITE_THROUGH, NULL );
	if( !com1_h ) {
		printf("Error: %d\n", GetLastError());
		return 0;
	}

	DCB comdcb = {0};
	//if(! BuildCommDCB("baud=460800 parity=N data=8 stop=1", &comdcb ) ){
	if(! BuildCommDCB("baud=115200 parity=N data=8 stop=1", &comdcb ) ){
	//if(! BuildCommDCB("baud=230400 parity=N data=8 stop=1", &comdcb ) ){
		printf("Error: %d, line = %d\n", GetLastError(), __LINE__);
		CloseHandle( com1_h );
		return 0;
	}
	SetCommState( com1_h, &comdcb );

	COMMTIMEOUTS to = {0};
	to.ReadIntervalTimeout = 50;
	to.ReadTotalTimeoutMultiplier = 1;
	to.ReadTotalTimeoutConstant = 500;
	to.WriteTotalTimeoutMultiplier = 10;
	to.WriteTotalTimeoutConstant = 500;
	
	if(! SetCommTimeouts( com1_h, &to ) ) {
		printf("Error: %d, line = %d\n", GetLastError(), __LINE__);
		CloseHandle( com1_h );
		return 0;
	}

	console_h = GetStdHandle(STD_INPUT_HANDLE);

	DWORD consolemode;
	GetConsoleMode( console_h, &consolemode );
	SetConsoleMode( console_h, consolemode & ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT) );

	PurgeComm( com1_h, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_TXABORT|PURGE_RXABORT );

	DWORD cmd = 0, read;
	while(1) {
		ReadConsole( console_h, &cmd, 1, &read, NULL );
		printf("Command: %c", cmd);
		if( cmd == 'h' ) { printf("Hello\n"); continue; }
		if( cmd == 'e' ) { TransmitCommChar( com1_h, 'e' ); readEcho(); continue; }
		if( cmd == 'd' ) { readData(); continue; }
		if( cmd == 'a' ) { 
				int addr = 0;
				buf[0]='a';
				SetConsoleMode( console_h, consolemode & (ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT) );
				printf("\nAddress: ");
				ReadConsole( console_h, &(buf[1]), 100, &read, NULL );
				buf[read]='\0';
				printf("\nRead %d: %s", read, buf);
				SetConsoleMode( console_h, consolemode & ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT) );
				WriteFile( com1_h, buf, 7, &read, NULL );
				readEcho();
				continue; 
		}
		if( cmd == 'f' ) { 
				TransmitCommChar( com1_h, 'f' );
				readEcho();
				continue;
		}
		if( cmd =='c' ) {
			printf("\nStart erasing\n");

				buf[0]='c';

				SetConsoleMode( console_h, consolemode & (ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT) );
				printf("\nAddress: ");
				ReadConsole( console_h, &(buf[1]), 100, &read, NULL );
				buf[read]='\0';
				printf("\nRead %d: %s", read, buf);
				SetConsoleMode( console_h, consolemode & ~(ENABLE_LINE_INPUT|ENABLE_ECHO_INPUT) );
				WriteFile( com1_h, buf, 7, &read, NULL );
				readEcho();
				continue; 
		}

		if( cmd =='p' ) {
			programData();
			continue;
		}
		if( cmd == 'z' ) break;
	}
	printf("\nExiting\n");
	CloseHandle( com1_h );

	return 0;
}

int waitOk(int max, int quiet = 0 ) {
	int count = 0;
	while(count < max ) {
		ReadFile( com1_h, buf, 64, &read, NULL);
		buf[read]='\0';
		if( read && strstr(buf,"Ok") ) { if(!quiet) printf(buf); return 1; }
		count++;
		if(!quiet) printf(".");
	}
return 0;
}
int readEcho() {
	ReadFile( com1_h, buf, 100, &read, NULL);
	buf[read]='\0';
	printf("\n%s", buf);
	if( strstr(buf,"Ok") ) return 1;
	return 0;
}

void readData() {

	INPUT_RECORD events[10]; 

	char filename_sz[MAX_PATH] = {0};
	DWORD keys = 0;

	//printf("Home dir: %s\n", getenv( "HOMEPATH" ) );
	//strcat( filename_sz, getenv( "HOMEDRIVE" ) );
	//strcat( filename_sz, getenv( "HOMEPATH" ) );
	strcat( filename_sz, "P:\\SoftDevelop\\Console\\PS3\\PS3JB\\MY\\mylog1.bin" );

	printf("Filename: %s\n", filename_sz );

	FILE *file1 = fopen( filename_sz, "w+b");
	if(!file1) perror("Error");
	
	printf("Start reading\n");

	TransmitCommChar( com1_h, 'd' );

	while( ReadFile( com1_h, buf, sizeof buf, &read, NULL) ) {
		if(read) {
			char *pos = strstr( buf, "START" );
			if( pos ) {
				printf("Found START\n");
				fwrite( pos+5, 1, read-(pos-buf+5), file1 );
				break;
			}
		}
	TransmitCommChar( com1_h, 'd' );
	}


	while( !keys && ReadFile( com1_h, buf, sizeof buf, &read, NULL) ) {

		printf("Read bytes: %d\n", read );

		if(read) 
			fwrite( buf, 1, read, file1 );

		PeekConsoleInput( console_h, events, sizeof events, &read );
		if( read ) {
			while(read) {
				read--;
				if( events[read].EventType == KEY_EVENT && events[0].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE ) keys = 1;
				//printf("Event: 0x%x\n", events[read].EventType );
			}
		}
	}

	fclose( file1 );

}


void testTime(HANDLE com1_h) {
	clock_t clk = 0;
	float time = 0;
	char buf[512];
	unsigned long n = 0, sum = 0;

	clk = clock();
	while( sum < sizeof buf * 1000 ) {
		if( ! ReadFile( com1_h, buf, sizeof buf, &n, 0 ) ) {
			printf("Error: %d, line = %d\n", GetLastError(), __LINE__);
			CloseHandle( com1_h );
			return;
		}
		sum += n;
	}
	printf("Total Data = %d\nTotal Time = %f\n", sum, float(clock()-clk)/CLOCKS_PER_SEC );
}

int eraseSector(int sector) {
	_snprintf( buf, 6, "%06x", sector );
	buf[6] = '\0';
	printf("Erase sector 0x%s\n", buf);
	TransmitCommChar(com1_h, 'c');
	WriteFile( com1_h, buf, 7, &read, NULL );
	if(!waitOk(10, false)) {
		printf("Erase failure! %s\n", buf); return 0;
	}
	return 1;
}

void programData() {

	char filename[] = "P:\\SoftDevelop\\Console\\PS3\\PS3JB\\MY\\mylog2patch.bin";
	//char filename[] = "P:\\SoftDevelop\\Console\\PS3\\PS3JB\\MY\\mylog71.bin";
	
	char logname[] = "P:\\SoftDevelop\\Console\\PS3\\PS3JB\\MY\\mylog3.log";
	FILE* file = fopen( filename, "rb");
	if(!file) { perror("file opening"); return; }
	FILE* log1 = fopen( logname, "w+b");
	if(!log1) { perror("file opening"); return; }

	unsigned char pbuf[512];
	int total = 0;

	printf("\nStart programming\n");
	fprintf( log1, "Start programming\n\n" );

	for(int a = 0; a < 128; a++) {

		printf("#Sector %06x, offset = 0x%06x\n\n",a<<16, a<<17);
		fprintf( log1, "#Sector %06x\n\n",a<<16);

		if(! eraseSector(a<<16) ) {  fclose(file); fclose(log1); return; }

		for(int b = 0; b < 256; b++) {

			fseek(file, (a<<16|b<<8)*2, SEEK_SET);
			fprintf( log1, "File offset 0x%06x\n", (a<<16|b<<8)*2 );
			printf( "File offset 0x%06x\n", (a<<16|b<<8)*2 );
			fflush( log1 );

			read = 0;
			while( !feof(file) && read < 512 )
				read = read + fread( pbuf+read, 1, 512-read, file );
			if( read < 512 ) { printf("Error! Read %d\n", read ); fclose(file); fclose(log1); return; }

			Sleep(1);
			TransmitCommChar(com1_h, 'p'); 

			if(!waitOk(5,true)) {
					fprintf( log1, "Error! Not ok! %d %s\n", read, buf );
					printf("Error! Not ok! %d %s\n", read, buf ); 
					fclose(file); fclose(log1); return; 
			}

			//Sleep(1);

			int write = 0; read = 0; 
			//DWORD read1, tries;
			//unsigned char check = 0;

			while(write<512) {
				//TransmitCommChar( com1_h, pbuf[write] );
				//FlushFileBuffers( com1_h );
				WriteFile( com1_h, pbuf+write, 512-write, &read, NULL );
				/*
				read1=0, tries = 0;
				while(!read1 && tries++<100) 
					ReadFile( com1_h, &check, 1, &read1, NULL);
				if( strncmp((char*)&check, (char*)&pbuf[write], 1) ) {
					printf("Error writing! check = 0x%08x, pbuf = 0x%08x\n", check, pbuf[write]); 
					fprintf(log1,"Error writing! check = %08x, pbuf = %08x\n", check, pbuf[write]); 
					fclose(file); fclose(log1);
					return;
				}
				write++;
				*/
				write+=read;
			}
			//printf("Written %d, read ok\n", write);
			fprintf( log1, "Written %d, read ok\n", write);

			if(!waitOk(5,true)) {
					fprintf( log1, "Error! Not ok! %d %s\n", read, buf );
					printf("Error! Not ok! %d %s\n", read, buf ); 
					fclose(file); fclose(log1); return; 
			}

			char *pos = strstr( buf, "Ok" );
			unsigned char xor1 = pos[2], xor2 = 0;
			for(int i=0; i<512; i++) xor2 = xor2 ^ pbuf[i];
			if(xor1!=xor2) { 
				fprintf( log1, "Error! Not ok xor, xor1 = %x, xor2 = %x\n", read, xor1, xor2 );
				printf("Error! Not ok xor, xor1 = %x, xor2 = %x\n", read, xor1, xor2 ); 
				fclose(file); fclose(log1); return; 
			}

			total+=512;
			fprintf( log1, "total = %08x, xor ok\n", total);
			fflush( log1 );
			//printf("total = %08x, xor ok\n", total);

			TransmitCommChar(com1_h, '.');
			Sleep(1);

			TransmitCommChar(com1_h, b);
			TransmitCommChar(com1_h, a);

			ReadFile( com1_h, buf, 2, &read, NULL);
			int addr = (a<<8)|b;
			if( strncmp((char*)&addr, buf, 2))  { 
				fprintf( log1, "Error! Addr cmp failed! %x == %x\n", *(DWORD*)buf, addr ); 
				printf("Error! Addr cmp failed! %x == %x\n", *(DWORD*)buf, addr ); 
				fclose(file); fclose(log1); return; 
			}

			TransmitCommChar(com1_h, '.');

			printf("Address 0x%06x\n", addr<<8);
			fprintf( log1, "Addrres: 0x%06x\n", addr<<8);
			fflush( log1 );

			/*
			int read2=0, tries = 0;
			while( read2 < 256 && tries++<20) {
				ReadFile( com1_h, buf, 256-read2, &read, NULL);
				read2+=read;
				//if( buf[read-1] != '.' ) {
					char *pos = strstr(buf, "ERR");
					if( pos ) {
						printf("Error! Timeout. data1 = %02x data = %02x, addr1 = %02x addr2 = %02x addr3 = %02x\n", (unsigned char)pos[3], (unsigned char)pos[4], (unsigned char)pos[5], (unsigned char)pos[6], (unsigned char)pos[7] );
						fprintf( log1, "Error! Timeout. data1 = %02x data = %02x, addr1 = %02x addr2 = %02x addr3 = %02x\n", (unsigned char)pos[3], (unsigned char)pos[4], (unsigned char)pos[5], (unsigned char)pos[6], (unsigned char)pos[7] );
						fclose(file); fclose(log1); return;
					}
				//}
			}
			
			DWORD read2 = 1;
			while( read2 ) {
				ReadFile( com1_h, buf, 8, &read2, NULL);
				TransmitCommChar(com1_h, '.');
				char *pos = strstr(buf, "TUS");
				if( pos ) {
					printf("Status. data1 = %02x data = %02x, addr1 = %02x addr2 = %02x addr3 = %02x\n", (unsigned char)pos[3], (unsigned char)pos[4], (unsigned char)pos[5], (unsigned char)pos[6], (unsigned char)pos[7] );
					fprintf( log1, "Status. data1 = %02x data = %02x, addr1 = %02x addr2 = %02x addr3 = %02x\n", (unsigned char)pos[3], (unsigned char)pos[4], (unsigned char)pos[5], (unsigned char)pos[6], (unsigned char)pos[7] );
					fflush(log1);
				}
			}
			*/

			//fprintf( log1, "Read back bytes %d\n", read2);
			//fflush( log1 );

			//printf(".");
			//fprintf( log1, "**************\n\n" );
			//fflush( log1 );

			if(! waitOk(10,true) ) {
				printf("Error!"); 
				fprintf( log1, "Error! No ok! %s\n", buf ); 
				char *pos = strstr(buf, "ERR");
				if( pos ) {
					printf("Error! Timeout. data1 = %02x data = %02x, addr1 = %02x addr2 = %02x addr3 = %02x\n", (unsigned char)pos[3], (unsigned char)pos[4], (unsigned char)pos[5], (unsigned char)pos[6], (unsigned char)pos[7] );
					fprintf( log1, "Error! Timeout. data1 = %02x data = %02x, addr1 = %02x addr2 = %02x addr3 = %02x\n", (unsigned char)pos[3], (unsigned char)pos[4], (unsigned char)pos[5], (unsigned char)pos[6], (unsigned char)pos[7] );
				}
				fclose(file); fclose(log1); return;
			}
			
			//Sleep(10);

			//fclose(file); return;
		}
		fprintf( log1, "Total %d\n", total);
		fflush( log1 );
		printf("Total %d\n", total);
		//Sleep(100);
	}

	fclose(file);
	fclose(log1);
}



