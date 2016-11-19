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


    struct camera_control_win32 {
      
      camera_control_win32( camera& cam, float angle, float delta ) : 

        _camera{ cam }, _angle{ angle }, _delta{ delta } {

        bind_input();

        _rotx_p = math::rotx< float >( angle );
        _rotx_n = math::rotx< float >( -angle );
        _roty_p = math::roty< float >( angle );
        _roty_n = math::roty< float >( -angle );
        _rotz_p = math::rotz< float >( angle );
        _rotz_n = math::rotz< float >( -angle );


        os::cursor_center();

        set_cursor_xy( os::cursor_xy() );
      }

      ~camera_control_win32() {

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
              if( not _cursor_on ) {
                set_is_cursor( is_cursor() != true );
                os::cursor_center();
              }
            break;

            case os::action::cursor_on :
              if( _cursor_on ) {
                _cursor_on = false;
                set_is_cursor( false );
                show_cursor();
              }
            break;

            default:;
          }

          return true;
        };

        _eid[ count++ ] = $event( "mouse_move", "camera", this ) {

          auto mouse_move_xy = vec2i{ event.x, event.y };

          if( is_cursor() ) return true;

          auto delta = mouse_move_xy - cursor_xy();

          if( delta[ 0 ] == 0 and delta[ 1 ] == 0 ) return true;

          if( delta[ 1 ] != 0 ) pitch( -delta[ 1 ] );

          if( delta[ 0 ] != 0 ) yaw( -delta[ 0 ] );

          _camera.normalize();

          os::cursor_center();

          return true;
        };

        _eid[ count++ ] = $event( "mouse_down", "camera", this ) {

          return true;
        };

        _eid[ count++ ] = $event( "mouse_up", "camera", this ) {

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

      }

      void unbind_input() {

        event::remove( "key_down", _eid[ 0 ] );
        event::remove( "key_up", _eid[ 1 ] );
        event::remove( "mouse_move", _eid[ 2 ] );
        event::remove( "mouse_down", _eid[ 3 ] );
        event::remove( "mouse_up", _eid[ 4 ] );
      }


      void pitch( int steps ) {

        int sign = math::sign( steps );
        steps = math::abs( steps );

        range{ 0, steps } $do { 

          sign > 0 ? _camera.pitch( _rotx_p ) : _camera.pitch( _rotx_n );
        };
      }

      void yaw( int steps ) {

        int sign = math::sign( steps );
        steps = math::abs( steps );

        if( _camera.rotation()[1][1] < 0 ) sign = -sign;

        range{ 0, steps } $do { 

          sign > 0 ? _camera.yaw( _roty_p ) : _camera.yaw( _roty_n );
        };
      }

      void roll( int steps ) {

        int sign = math::sign( steps );
        steps = math::abs( steps );

        range{ 0, steps } $do { 

          sign > 0 ? _camera.roll( _rotz_p ) : _camera.roll( _rotz_n );
        };
      }

      bool is_cursor() const { return _is_cursor; }

      void show_cursor() { 
        
        os::show_cursor( _is_cursor );
      }

      void set_is_cursor( bool flag ) { 

        if( _is_cursor != flag ) {

          _is_cursor = flag; 

          show_cursor();
        }
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
      event::eid_t _eid[5]{};
      os::action _action[8];
      bool _is_cursor{ true };
      bool _cursor_on{};
    };


  }
}



