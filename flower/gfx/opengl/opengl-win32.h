#pragma once

#include "os/windows.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "os/types.h"
#include "os/error-win32.h"
#include "os/window.h"
#include "gl/gl.h"
#include "renderer.h"


namespace lib {

  namespace gfx {

  namespace gl {


    struct opengl_win32 : nocopy {


      opengl_win32( os::hwnd hwnd ) {

        {
          auto w_tmp = os::window::create( "Opengl Context", 1, 1 );

          _hwnd = w_tmp.handle();

          context_temp_create();

          wgl::api_init();
        
          context_destroy( true );

          _hwnd = os::hwnd{};
        }

        _hwnd = hwnd;

        context_create( 3, 2 );

        gl::api_init();

        log::gfx, "Vendor: ", gl::GetString( GL_VENDOR ), log::endl;
        log::gfx, "Renderer: ", gl::GetString( GL_RENDERER ), log::endl;
        log::gfx, "Version: ", gl::GetString( GL_VERSION ), log::endl;
        log::gfx, "GLSL Version: ", gl::GetString( GL_SHADING_LANGUAGE_VERSION ), log::endl;
      }

      ~opengl_win32() {

        context_destroy();
        
        _hwnd = os::hwnd{};
      }

      void operator()() const {

        process_queue();

        SwapBuffers( _dc );
      };

      void context_temp_create() {

        log::gfx, "create temporary opengl context", log::endl;

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

      void context_create( int version_major, int version_minor, bool core_profile = true ) {

        log::gfx, "create opengl context version ", version_major, ".", version_minor, log::endl;

        _dc = GetDC( _hwnd );

        if( not _dc ) $throw $error_win32( "getdc failed" );

        int const pixel_fmt_i[]{  
          WGL_DRAW_TO_WINDOW_ARB, GL_TRUE, 
          WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
          WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
          WGL_COLOR_BITS_ARB, 32,
          WGL_DEPTH_BITS_ARB, 24,
          WGL_STENCIL_BITS_ARB, 8, 
          WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, 
          0 };

        float const pixel_fmt_f[]{ 0 };

        int pf = -1; uint pf_size = 0;

        auto result = wgl::ChoosePixelFormat( _dc, pixel_fmt_i, pixel_fmt_f, 1, &pf, &pf_size );

        if( not result ) $throw $error_win32( "set pixel format failed" );

        PIXELFORMATDESCRIPTOR pfd{};

        result = SetPixelFormat( _dc, pf, &pfd );

        if( not result ) $throw $error_win32( "set pixel format failed" );

        int const context_attribs[]{  
          WGL_CONTEXT_MAJOR_VERSION_ARB, version_major, 
          WGL_CONTEXT_MINOR_VERSION_ARB, version_minor, 
          WGL_CONTEXT_PROFILE_MASK_ARB, ( core_profile ? 
          WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB ),
          WGL_CONTEXT_FLAGS_ARB, 0, 
          0 };

        _glc = wgl::CreateContextAttribs( _dc, os::hglrc{}, context_attribs );

        if( not _glc ) $throw $error_win32( "wglCreateContext failed" );

        result = $glMakeCurrent( _dc, _glc );

        if( not result ) $throw $error_win32( "wglMakeCurrent failed" );
      }


      void context_destroy( bool temp = false ) {

        log::gfx, "destroy ", ( temp ? "temporary" : "" ), " opengl context", log::endl;

        $glMakeCurrent( _dc, os::hglrc{} );

        $glDeleteContext( _glc );

        ReleaseDC( _hwnd, _dc );

        _dc = os::hdc{};
      }

      void init() {

        gl::FrontFace( GL_CW );
        gl::Enable( GL_TEXTURE_CUBE_MAP_SEAMLESS );
        gl::ClearColor( 0.f, 0.f, 0.f, 1.f );
        gl::ClearDepthf( 0.f );
        gl::ClearStencil( 0 );
        gl::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
      }

      os::hwnd _hwnd{};
      os::hdc _dc{};
      os::hglrc _glc{};
    };

  }
  }
}


