#pragma once

#include "lib/config.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "os/types.h"

#if defined( $CONFIG_MSC ) || defined( $CONFIG_MINGW )

  /*
  extern "C" void* APIENTRY wglGetProcAddress( cstr );
  extern "C" void* APIENTRY wglCreateContext( os::hdc );
  extern "C" void* APIENTRY wglMakeCurrent( os::hdc, os::hglrc );
  extern "C" void* APIENTRY wglDeleteContext( os::hglrc );
  extern "C" void* APIENTRY SwapBuffers( os::hdc );
  */

  #include "os/windows.h"

  #ifndef APIENTRY
    #define APIENTRY __stdcall
  #endif

  #define $getProcAddress( ... ) GetProcAddress( __VA_ARGS__ )
  #define $loadOpenglLib() LoadLibraryA( "opengl32.dll" )
  #define $freeOpenglLib( ... ) FreeLibrary( __VA_ARGS__ )
  #define $glGetProcAddress( ... ) wglGetProcAddress( __VA_ARGS__ )
  #define $glCreateContext( ... ) wglCreateContext( __VA_ARGS__ )
  #define $glMakeCurrent( ... ) wglMakeCurrent( __VA_ARGS__ )
  #define $glDeleteContext( ... ) wglDeleteContext( __VA_ARGS__ )
  #define $SwapBuffers( ... ) SwapBuffers( __VA_ARGS__ )

#endif


namespace lib {

  namespace gfx {

  namespace gl {



  }
  }
}


