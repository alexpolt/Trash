#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "math/common.h"
#include "event/common.h"
#include "os/action.h"


namespace lib {

  namespace render {


    struct camera {
      
      void move( vec3f const& delta ) { _pos += delta; }

      void rotate( mat3f const& r ) { _rot = r * rot; }

      auto position() const { return _pos; }

      auto rotation() const { return _rot; }

      auto set_position( vec3f pos ) { _pos = pos; }

      auto set_rotation( mat3f rot ) { _rot = rot; }

      void normalize() { _rot = normalize( _rot ); }

      vec3f _pos{};
      mat3f _rot{};
    };


    struct camera_fpv : camera {
      
      camera_fpv( float angle, float delta, bool bind = false ) : _angle{ angle }, _delta{ delta } {

        if( bind ) bind_input();

        _rotx_p = math::rotx< float >( angle );
        _rotx_n = math::rotx< float >( -angle );
        _roty_p = math::roty< float >( angle );
        _roty_n = math::roty< float >( -angle );
        _rotz_p = math::rotz< float >( angle );
        _rotz_n = math::rotz< float >( -angle );
      }

      ~camera_fpv() {

        if( _eid[ 1 ] ) unbind_input();
      }

      void bind_input() {

        static struct {
          camera_fpv* camera;
          vec2i mouse_down_xy;
          bool cursor_off;
          bool bound;
        } data{ this };

        int count = 0;

        _eid[ count++ ] = $event( "key_down", "camera" ) {

          switch( event.action ) {

            case os::action::left : data.camera->move( vec3f{ -delta, 0, 0 } ); break;
            case os::action::right : data.camera->move( vec3f{ delta, 0, 0 } ); break;
            case os::action::up : data.camera->move( vec3f{ 0, 0, delta } ); break;
            case os::action::down : data.camera->move( vec3f{ 0, 0, -delta } ); break;
            case os::action::bank_left : data.camera->rotatez( 1 ); break;
            case os::action::bank_right : data.camera->rotatez( -1 ); break;
            case os::action::cursor_off : data.cursor_off = true;
          }

          return true;
        };

        _eid[ count++ ] = $event( "key_up", "camera" ) {

          switch( event.action ) {

            case os::action::cursor : 
              data.camera->set_is_cursor( data.camera->is_cursor() != true ); break;
            case os::action::cursor_off : data.cursor_off = false; break;
          }

          return true;
        };

        _eid[ count++ ] = $event( "mouse_up", "camera", &data ) {

          return true;
        };
        
        _eid[ count++ ] = $event( "mouse_down", "camera", &data ) {

          data.mouse_down_xy = vec2i{ event.x, event.y };
  
          return true;
        };

        _eid[ count++ ] = $event( "mouse_move", "camera", &data ) {
 
          auto mouse_move_xy = vec2i{ event.x, event.y };

          if( data.camera->is_cursor() and not data.cursor_off ) {

            data.camera->set_cursor_xy( mouse_move_xy );

            return true;
          }

          auto delta = mouse_move_xy - data.mouse_down_xy;

          data.mouse_down_xy = mouse_move_xy;

          if( delta[ 0 ] != 0 ) data.camera->rotatey( math::sign( delta[ 0 ] ) );
          if( delta[ 1 ] != 0 ) data.camera->rotatex( math::sign( delta[ 1 ] ) );

          data.camera->normalize();

          return true;
        };

        data.bound = true;
      }

      void unbind_input() {

        event::remove( "key_down",    _eid[ 0 ] );
        event::remove( "key_up",      _eid[ 1 ] );
        event::remove( "mouse_up",    _eid[ 2 ] );
        event::remove( "mouse_down",  _eid[ 3 ] );
        event::remove( "mouse_move",  _eid[ 4 ] );
      }

      void rotatex( int sign ) { sign > 0 ? rotate( _rotx_p ) : rotate( _rotx_n ); }
      void rotatey( int sign ) { sign > 0 ? rotate( _roty_p ) : rotate( _roty_n ); }
      void rotatez( int sign ) { sign > 0 ? rotate( _rotz_p ) : rotate( _rotz_n ); }

      bool is_cursor() const { return _is_cursor; }
      vec2i cursor_xy() const { return _cursor_xy; }

      void set_is_cursor( bool flag ) { _is_cursor = flag; }
      void set_cursor_xy( vec2i xy ) { _cursor_xy = xy; }

      float _angle{};
      float _delta{};
      mat3f _rotx_p{};
      mat3f _rotx_n{};
      mat3f _roty_p{};
      mat3f _roty_n{};
      mat3f _rotz_p{};
      mat3f _rotz_n{};
      vec2i _cursor_xy{};
      event::eid_t _eid[5]{};
      bool _is_cursor{};
    };


  }
}



