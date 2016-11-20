#pragma once

#include "os/windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "os/types.h"
#include "os/error-win32.h"
#include "gl/gl.h"
#include "gl/api.h"
#include "renderer.h"


namespace lib {

  namespace gfx {

  namespace gl {


    struct opengl_win32 : nocopy {


      opengl_win32( os::hwnd hwnd ) : _hwnd{ hwnd } {

        context_create();

        gl_api_init();

        log::gfx, "Vendor: ", gl::GetString( GL_VENDOR ), log::endl;
        log::gfx, "Renderer: ", gl::GetString( GL_RENDERER ), log::endl;
        log::gfx, "Version: ", gl::GetString( GL_VERSION ), log::endl;
      }

      ~opengl_win32() {

        context_destroy();
      }

      void operator()() const {

        renderer();

        SwapBuffers( _dc );
      };

      void context_create() {

        log::gfx, "create opengl context", log::endl;

        _dc = GetDC( _hwnd );

        if( not _dc ) $throw $error_win32( "getdc failed" );

        PIXELFORMATDESCRIPTOR pfd{};

        pfd.nSize = $size( pfd );
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        auto pf = ChoosePixelFormat( _dc, &pfd );

        bool result = SetPixelFormat( _dc, pf, &pfd );

        if( not result ) $throw $error_win32( "set pixel format failed" );

        _glc = $glCreateContext( _dc );

        if( not _glc ) $throw $error_win32( "wglCreateContext failed" );

        result = $glMakeCurrent( _dc, _glc );

        if( not result ) $throw $error_win32( "wglMakeCurrent failed" );
      }

      void context_destroy() {

        log::gfx, "destroy opengl context", log::endl;

        $glMakeCurrent( _dc, NULL );

        $glDeleteContext( _glc );

        ReleaseDC( _hwnd, _dc );
      }

      void init() {

        gl::ClearColor( 0.f, 0.f, 0.f, 1.f );
        gl::ClearDepthf( 0.f );
        gl::ClearStencil( 0 );
        gl::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
      }

      os::hwnd _hwnd;
      os::hdc _dc;
      os::hglrc _glc;
    };

  }
  }
}


