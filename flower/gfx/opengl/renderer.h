#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/log.h"
#include "gl/gl.h"
#include "render/common.h"


namespace lib {

  namespace gfx {

    using namespace render;

    inline void message( messages::clear& );
    inline void message( messages::vbuffer& );

    inline void process_queue() {

      if( available() ) {

        auto msg = render::pop();

        log::gfx, msg, " render", log::endl;

        switch( msg.type() ) {

          case message::type::clear: message( *(messages::clear*) msg.data() ); break;
          case message::type::vbuffer: message( *(messages::vbuffer*) msg.data() ); break;

          default:

          $assert( false, "unknown message" );
        }
      }

    }


    inline void message( messages::clear& msg ) {

      log::gfx, msg, log::endl;

      gl::ClearColor( msg.color.rf(), msg.color.gf(), msg.color.bf(), msg.color.af() );

      if( msg.depth != -1.f ) {

        gl::ClearDepthf( msg.depth );

        gl::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      } else {

        gl::Clear( GL_COLOR_BUFFER_BIT );
      }
    }


    inline void message( messages::vbuffer& msg ) {

      log::gfx, msg, log::endl;


    }


  }
}



