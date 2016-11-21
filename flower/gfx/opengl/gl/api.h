#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "gl.h"
#include "error.h"

#define GL_GLEXT_PROTOTYPES
#define WGL_WGLEXT_PROTOTYPES

#include "glcorearb.h"
#include "wglext.h"


namespace lib {

  namespace gfx {

  namespace gl {


    void gl_api_init();


    #include "api-list.h"


    enum class api {

      #define $gl_api_enum( $0 ) $0,
      $gl_api_list ( $gl_api_enum )
      size
    };


    TP<TN...>
    void* api_fn_ptrs[ (ssize_t) api::size ];

    TP<TN...>
    cstr api_fn_names[ (ssize_t) api::size ] {

      #define $gl_api_name( $0 ) $str( $paste( gl, $0 ) ),
      $gl_api_list ( $gl_api_name )
    };


    TP<TN T> struct map { using type = T; };
    TP<> struct map< int > { using type = uint; };
    TP<TN T> using map_t = typename map< T >::type;

    TP<TN... TT, TN... UU, TN R0, TN R1>
    void check_args( R0 (APIENTRY *)(TT...), R1(UU...) ) {
        
      static_assert( all_v< is_same_v< map_t< TT >, map_t< UU > >... >, "invalid arguments to a gl function"  );
    }

    #define $gl_api_thunk( $0 ) \
                                \
    TP<TN... TT, TN R = decltype( ::$paste( gl, $0 )( declval< TT >()...) ) > \
    R $0( TT... args ) {                                                      \
      using fn_orig_ptr = decltype( ::$paste( gl, $0 ) )*;                 \
      using fn_ptr = R (*)( TT... );                                          \
      check_args( fn_orig_ptr{}, fn_ptr{} );                                  \
      return fn_orig_ptr( api_fn_ptrs<>[ (ssize_t) api::$0 ] )( args... );    \
    }

    $gl_api_list( $gl_api_thunk )

    void gl_api_init() {

      static decltype( $loadOpenglLib() ) handle{};

      log::gfx, "load gl entry points", log::endl;

      ssize_t index = 0;

      for( auto name : api_fn_names<> ) {

        log::gfx, "load ", name;

        void* ptr = (void*) $glGetProcAddress( name );

        ssize_t n = (ssize_t) ptr;

        if( n == 0 or n == 1 or 
            n == 2 or n == 2 or n == -1 ) {
          
          if( not handle ) handle = $loadOpenglLib();

          if( not handle ) $throw $error_api( name );

          ptr = (void*) $getProcAddress( handle, name );

          if( not ptr ) $throw $error_api( name );
        }

        log::gfx, " = ", ptr, log::endl;

        api_fn_ptrs<>[ index++ ] = ptr;
      }

      log::gfx, "loading finished", log::endl;
    }


  }
  }
}


