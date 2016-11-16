#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/to-string.h"
#include "math/common.h"
#include "event/common.h"
#include "os/action.h"
#include "os/cursor.h"


namespace lib {

  namespace render {


    struct camera {
      
      void move( vec3f const& delta ) { _pos = _pos + _rot * delta; }

      void move_pos( vec3f const& delta ) { _pos = _pos + delta; }

      void rotate( mat3f const& r ) { _rot = r * _rot; }
      
      void yaw( mat3f const& r ) { _rot = r * _rot; }
      void pitch( mat3f const& r ) { _rot = _rot * r; }
      void roll( mat3f const& r ) { _rot = _rot * r; }

      auto& position() const { return _pos; }

      auto& rotation() const { return _rot; }

      auto set_position( vec3f pos ) { _pos = pos; }

      auto set_rotation( mat3f rot ) { _rot = rot; }

      void normalize() { _rot = math::normalize( _rot ); }

      cstr to_string() const { 
        
        return lib::to_string( "camera( %+.5f, %+.5f, %+.5f )", _pos[ 0 ], _pos[ 1 ], _pos[ 2 ] );
      }

      vec3f _pos{ 0.f };
      mat3f _rot{ 1.f };
    };


    struct camera_control {
      
      camera_control( camera& cam, float angle, float delta ) : 

        _camera{ cam }, _angle{ angle }, _delta{ delta } {

        bind_input();

        _rotx_p = math::rotx< float >( angle );
        _rotx_n = math::rotx< float >( -angle );
        _roty_p = math::roty< float >( angle );
        _roty_n = math::roty< float >( -angle );
        _rotz_p = math::rotz< float >( angle );
        _rotz_n = math::rotz< float >( -angle );

        show_cursor();

        os::cursor_center();

        set_cursor_xy( os::cursor_xy() );
      }

      ~camera_control() {

        unbind_input();
      }

      void bind_input() {

        int count = 0;

        _eid[ count++ ] = $event( "key_down", "camera", this ) {

          switch( event.action ) {
            case os::action::left : _action[ 0 ] = event.action; break;
            case os::action::right : _action[ 1 ] = event.action; break;
            case os::action::up : _action[ 2 ] = event.action; break;
            case os::action::down : _action[ 3 ] = event.action; break;
            case os::action::jump : _action[ 6 ] = event.action; break;
            case os::action::crouch : _action[ 7 ] = event.action; break;
            default:;
          }

          switch( event.action ) {

            case os::action::cursor_on :
              if( not is_cursor() ) {
                _cursor_on = true;
                set_is_cursor( true );
                show_cursor();
              }
            break;

            default:;
          }

          return true;
        };

        _eid[ count++ ] = $event( "key_up", "camera", this ) {

          switch( event.action ) {
            case os::action::left : _action[ 0 ] = os::action::null; break;
            case os::action::right : _action[ 1 ] = os::action::null; break;
            case os::action::up : _action[ 2 ] = os::action::null; break;
            case os::action::down : _action[ 3 ] = os::action::null; break;
            case os::action::jump : _action[ 6 ] = os::action::null; break;
            case os::action::crouch : _action[ 7 ] = os::action::null; break;
            default:;
          }

          switch( event.action ) {

            case os::action::cursor :
              set_is_cursor( is_cursor() != true );
              show_cursor();
              os::cursor_center();
            break;

            case os::action::cursor_on :
              if( _cursor_on ) {
                _cursor_on = false;
                set_is_cursor( false );
                show_cursor();
                os::cursor_center();
              }
            break;

            default:;
          }

          return true;
        };

        _eid[ count++ ] = $event( "mouse_move", "camera", this ) {

          auto mouse_move_xy = vec2i{ event.x, event.y };

          if( is_cursor() ) {

            //set_cursor_xy( mouse_move_xy );

            return true;
          }

          auto delta = mouse_move_xy - cursor_xy();

          //set_cursor_xy( mouse_move_xy );

          if( delta[ 1 ] != 0 ) pitch( math::sign( -delta[ 1 ] ) );

          if( delta[ 0 ] != 0 ) yaw( math::sign( -delta[ 0 ] ) );

          _camera.normalize();

          return true;
        };

      }

      void operator()() {

        for( auto action : _action )
        switch( action ) {
          case os::action::left : _camera.move( vec3f{ -delta(), 0, 0 } ); break;
          case os::action::right : _camera.move( vec3f{ delta(), 0, 0 } ); break;
          case os::action::up : _camera.move( vec3f{ 0, 0, delta() } ); break;
          case os::action::down : _camera.move( vec3f{ 0, 0, -delta() } ); break;
          case os::action::jump : _camera.move_pos( vec3f{ 0, delta(), 0 } ); break;
          case os::action::crouch : _camera.move_pos( vec3f{ 0, -delta(), 0 } ); break;
          default:;
        }

        if( not is_cursor() ) os::cursor_center();
      }

      void unbind_input() {

        event::remove( "key_down", _eid[ 0 ] );
        event::remove( "key_up", _eid[ 1 ] );
        event::remove( "mouse_move", _eid[ 2 ] );
      }


      void pitch( float sign ) { sign > 0 ? _camera.pitch( _rotx_p ) : _camera.pitch( _rotx_n ); }

      void yaw( float sign ) { sign > 0 ? _camera.yaw( _roty_p ) : _camera.yaw( _roty_n ); }

      void roll( float sign ) { sign > 0 ? _camera.roll( _rotz_p ) : _camera.roll( _rotz_n ); }

      bool is_cursor() const { return _is_cursor; }

      void show_cursor() { 

        os::show_cursor( _is_cursor );
      }

      void set_is_cursor( bool flag ) { 

        _is_cursor = flag; 

        show_cursor();
      }

      vec2i const& cursor_xy() const { return _cursor_xy; }

      void set_cursor_xy( vec2i xy ) { _cursor_xy = xy; }

      float delta() const { return _delta; }
      float angle() const { return _angle; }

      camera& _camera;
      float _angle{};
      float _delta{};
      mat3f _rotx_p{};
      mat3f _rotx_n{};
      mat3f _roty_p{};
      mat3f _roty_n{};
      mat3f _rotz_p{};
      mat3f _rotz_n{};
      vec2i _cursor_xy{};
      event::eid_t _eid[3]{};
      os::action _action[8];
      bool _is_cursor{};
      bool _cursor_on{};
    };


  }
}



