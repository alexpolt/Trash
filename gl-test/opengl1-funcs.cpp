#include <gl/gl.h>
#include "wglext.h"
#include "glext.h"
#include <initializer_list>

HGLRC glrc;
DEVMODE dm; 

void opengl_print_errors() {
  while( GLenum error = glGetError() ) {
    if( error == GL_NO_ERROR ) break;
    switch( error ) { 
      case GL_INVALID_ENUM: printf("*error: gl_invalid_enum\n"); break;
      case GL_INVALID_OPERATION: printf("*error: gl_invalid_enum\n"); break;
      case GL_INVALID_VALUE: printf("*error: gl_invalid_enum\n"); break;
      default:;
    }
  }
}
void APIENTRY opengl_error(GLenum, GLenum, GLuint, GLenum, GLsizei length, const char *message, const void* ) {
  printf("*opengl_error: %s\n", message);
}

#include "opengl1-register.cpp"

#include "opengl1-draw.cpp"

WNDPROC opengl_oldwndproc;
LRESULT CALLBACK opengl_wndproc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { 
  if(msg==WM_PAINT) { ValidateRect( hwnd, NULL ); return 0; }
  if(msg==WM_ERASEBKGND) { return 0; }
  return CallWindowProc( opengl_oldwndproc, hwnd, msg, wparam, lparam); 
}

bool opengl_begin( HWND hwnd, bool subclass = true ) {
  printf("OpenGL init\n");

  PIXELFORMATDESCRIPTOR pfd = 
  { sizeof (PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, 0, 0, 0, 0, 0 };

  auto hdc = raii( GetDC( hwnd ), [hwnd]( HDC hdc ) { printf("release dc\n"); ReleaseDC( hwnd, hdc ); } );
  if(! hdc) return false;
  int pf = ChoosePixelFormat( hdc, &pfd );
  if( !pf ) return false;
  if( ! SetPixelFormat( hdc, pf, &pfd ) ) return false;

  opengl_oldwndproc = (WNDPROC) GetWindowLongPtr( hwnd, GWLP_WNDPROC ) ;
  printf("wndproc= 0x%x\n", (int)opengl_oldwndproc );
  if( subclass && ! SetWindowLongPtr( hwnd, GWLP_WNDPROC, (LONG_PTR) opengl_wndproc )  ) return false;

  glrc = wglCreateContext( hdc );
  if(!glrc) return false;

  if(! wglMakeCurrent( hdc, glrc ) ) return false; 
  printf("%s, %s\n", glGetString( GL_RENDERER ), glGetString( GL_VERSION ) );

  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress( "wglCreateContextAttribsARB" );

  if( wglCreateContextAttribsARB ) {
    printf("Create new context\n");

    PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC) wglGetProcAddress("glDebugMessageCallback");
    if(glDebugMessageCallback) {
      printf("Setup debug callback\n");
      glDebugMessageCallback( opengl_error, nullptr );
    }

    int attribs[] = { WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,  
                            WGL_CONTEXT_FLAGS_ARB, 0, 0 };
    HGLRC glrc2 = wglCreateContextAttribsARB( hdc, 0, attribs );
    if(! glrc2 ) {
      printf("Failed new context\n");
      opengl_print_errors();
      return false;
    } else {
      wglMakeCurrent( 0, 0 );
      wglDeleteContext( glrc );
      wglMakeCurrent( hdc, glrc2 );
      printf("new context\n%s\n", glGetString( GL_VERSION ) );
      glrc = glrc2;
    }
  } else {
    printf("function createctxattribts not found\n");
  }
  opengl_print_errors();

  if( ! opengl_register() ) return false;

  dm.dmSize = sizeof(dm);
  dm.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH | DM_BITSPERPEL; 
  EnumDisplaySettings( 0, ENUM_CURRENT_SETTINGS, &dm );

  if( ChangeDisplaySettingsEx( NULL, &dm, NULL, CDS_FULLSCREEN, NULL ) != DISP_CHANGE_SUCCESSFUL ) {
    printf("CDS failed\n");
    return false;
  }

  return opengl_setup();

}

void opengl_end() {
  printf("OpenGL stop\n");
  opengl_release();

  ChangeDisplaySettingsEx( NULL, &dm, NULL, CDS_RESET, NULL );

  if( glrc ) {
    wglMakeCurrent( 0, 0 );
    wglDeleteContext ( glrc );
  }
}





