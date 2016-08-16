

all: main.cpp directx.cpp vs1.hlsl vs2.hlsl ps1.hlsl ps2.hlsl makefile
	fxc /Emain /Tvs_3_0 /O1 /Fh vs1.h vs1.hlsl
	fxc /Emain /Tps_3_0 /O1 /Fh ps1.h ps1.hlsl
	fxc /Emain /Tps_3_0 /O1 /Fh ps2.h /Vnps2_binary ps2.hlsl
	g++ -static -mtune=pentium4 -U__STRICT_ANSI__ -D_X86_ -Wall -std=c++14 -O2 -msse2 -ffast-math -o main.exe main.cpp -ld3d9 -ldxerr9 -lwinmm


