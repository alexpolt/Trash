# Microsoft Developer Studio Project File - Name="Wave1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Wave1 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Wave1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Wave1.mak" CFG="Wave1 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Wave1 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Wave1 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Wave1 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "P:\Microsoft DirectX SDK (Summer 2004)\Include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3d9.lib dxerr9.lib d3dx9.lib Winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\Microsoft Visual Studio 6.0\VC98\_Lib" /libpath:"P:\Microsoft DirectX SDK (Summer 2004)\Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Wave1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Wave1___Win32_Release"
# PROP BASE Intermediate_Dir "Wave1___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "P:\Microsoft DirectX SDK (Summer 2004)\Include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G5 /ML /W3 /I "P:\Microsoft DirectX SDK (Summer 2004)\Include" /D "WIN32" /D "_CONSOLE" /Yu"stdafx.h" /o1 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d3d9.lib dxerr9.lib d3dx9.lib Winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"C:\Program Files\Microsoft Visual Studio 6.0\VC98\_Lib" /libpath:"P:\Microsoft DirectX SDK (Summer 2004)\Lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 msvcrt.lib kernel32.lib user32.lib d3d9.lib d3dx9d.lib Winmm.lib "P:\Microsoft DirectX SDK (Summer 2004)\Lib\dxerr9.lib" /nologo /subsystem:console /incremental:no /machine:IX86 /nodefaultlib /libpath:"P:\Microsoft DirectX SDK (August 2009)\Lib\x86" /align:16 /merge:.reloc=.rdata /merge:.rdata=.data
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Wave1 - Win32 Debug"
# Name "Wave1 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Wave1.cpp
DEP_CPP_WAVE1=\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3d9.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3d9caps.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3d9types.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9anim.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9core.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9effect.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9math.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9math.inl"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9mesh.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9shader.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9shape.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9tex.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\d3dx9xof.h"\
	"..\..\..\microsoft directx sdk (summer 2004)\include\dxerr9.h"\
	".\StdAfx.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
