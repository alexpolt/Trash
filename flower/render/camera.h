#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"
#include "math/common.h"


namespace lib {

  namespace render {


    struct camera {
      
      camera( vec3f pos = vec3f{}, mat3f rot = mat3f{ 1.f } ) : _pos{ pos }, _rot{ rot } { }

      void move( vec3f const& delta ) { _pos = _pos + _rot * delta; }
      void move_pos( vec3f const& delta ) { _pos = _pos + delta; }
      void rotate( mat3f const& r ) { _rot = r * _rot; }
      
      void yaw( mat3f const& r ) {  _rot = r * _rot; }
      void pitch( mat3f const& r ) { _rot = _rot * r; }
      void roll( mat3f const& r ) { _rot = _rot * r; }

      void yaw( float a ) {  _rot = math::roty< float >( a ) * _rot; }
      void pitch( float a ) { _rot = _rot * math::rotx< float >( a ); }
      void roll( float a ) { _rot = _rot * math::rotz< float >( a ); }

      auto& position() const { return _pos; }
      auto& rotation() const { return _rot; }

      auto set_position( vec3f pos ) { _pos = pos; }
      auto set_rotation( mat3f rot ) { _rot = rot; }

      void normalize() { _rot = math::normalize( _rot ); }

      cstr to_string() const { 
        
        return lib::to_string( "camera( %+.5f, %+.5f, %+.5f )", _pos[ 0 ], _pos[ 1 ], _pos[ 2 ] );
      }

      vec3f _pos{};
      mat3f _rot{ 1.f };
   };



  }
}



