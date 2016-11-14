#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "math/common.h"


namespace lib {

  namespace render {

    struct camera {
      
      camera( bool bind = false ) {

        if( bind ) bind_input();
      }

      void bind_input() {

        $event( "key_up" ) {

          return true;
        };

        $event( "mouse_move" ) {

          return true;
        };
      }

      void move( vec3f delta ) { pos += delta; }

      void rotate( mat3f r ) { rot = r * rot; }

      void rotatex( float rad ) { rot = normalize( math::rotx( rad ) ) * rot; }
      void rotatey( float rad ) { rot = normalize( math::roty( rad ) ) * rot; }
      void rotatez( float rad ) { rot = normalize( math::rotz( rad ) ) * rot; }

      auto position() const { return pos; }
      auto rotation() const { return rot; }

      mat3f rot;
      vec3f pos;
    };

  }
}


