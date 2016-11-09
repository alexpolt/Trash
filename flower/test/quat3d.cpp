
#include "lib/common.h"
#include "os/common.h"
#include "event/common.h"
#include "math/common.h"


const int w = 1000, h = 1000;

uint data[ h ][ w ];

int index=1, ww, mul = 1, iter = 15;

vec2i xy_prev;

bool rotate, loop = true, drawing = false;

auto rot = mat3f{ 1 };

float s = math::pi12f / 25.f;

mat3f rotx[2]{ math::rotx(  s ), math::rotx( -s ) };
mat3f roty[2]{ math::roty( -s ), math::roty( s ) };


void data_clear() {
  for( auto y : range{ 0, h } )
  for( auto x : range{ 0, w } ) {
    data[ y ][ x ] = 0;
  }
}

TP<TN T0, ssize_t... NN >
constexpr auto qmul1( lib::math::vec_t< T0, NN... > l, lib::math::vec_t< T0, NN... > right ) {

  auto m = lib::math::mat< T0, 4 > {

    lib::math::vec< T0, 4 >{ l[0], -l[1],  l[2], -l[3] },
    lib::math::vec< T0, 4 >{ l[1],  l[0], -l[3], -l[2] },
    lib::math::vec< T0, 4 >{-l[2],  l[3],  l[0], -l[1] },
    lib::math::vec< T0, 4 >{ l[3],  l[2],  l[1],  l[0] },
  };

  return m * right;
}

TP<TN T0, ssize_t... NN >
constexpr auto qmul2( lib::math::vec_t< T0, NN... > l, lib::math::vec_t< T0, NN... > right ) {

  auto m = lib::math::mat< T0, 4 > {

    lib::math::vec< T0, 4 >{ l[0], -l[1], -l[2], -l[3] },
    lib::math::vec< T0, 4 >{ l[1],  l[0],  l[3], -l[2] },
    lib::math::vec< T0, 4 >{ l[2],  l[3], -l[0],  l[1] },
    lib::math::vec< T0, 4 >{ l[3], -l[2],  l[1],  l[0] },
  };

  return m * right;
}

TP<TN T0, ssize_t... NN >
constexpr auto qmul3( lib::math::vec_t< T0, NN... > l, lib::math::vec_t< T0, NN... > right ) {

  auto m = lib::math::mat< T0, 4 > {

    lib::math::vec< T0, 4 >{ l[0], -l[1], -l[3],  l[2] },
    lib::math::vec< T0, 4 >{ l[1],  l[0], -l[2], -l[3] },
    lib::math::vec< T0, 4 >{ l[2], -l[3],  l[0], -l[1] },
    lib::math::vec< T0, 4 >{ l[3],  l[2],  l[1],  l[0] },
  };

  return m * right;
}

TP<TN T0, ssize_t... NN >
constexpr auto qmul4( lib::math::vec_t< T0, NN... > l, lib::math::vec_t< T0, NN... > right ) {

  auto m = lib::math::mat< T0, 4 > {

    lib::math::vec< T0, 4 >{ l[0], -l[1], -l[3],  l[2] },
    lib::math::vec< T0, 4 >{ l[1],  l[0], -l[2], -l[3] },
    lib::math::vec< T0, 4 >{-l[2], -l[3],  l[0], -l[1] },
    lib::math::vec< T0, 4 >{ l[3],  l[2],  l[1],  l[0] },
  };

  return m * right;
}




void draw( os::window& wnd ) {

  const int r = 50;
  const int rz = 50;

  for( int zz : range{ 0, rz } )

  for( int yy : range{ 0, r } )

    for( int xx : range{ 0, r } ) {

      float x = (2.f * xx) / r - 1.f;
      float y = (2.f * yy) / r - 1.f;
      float z = (2.f * zz) / rz - 1.f;

      float a = 25.f; 

      float rnd1 = float( rand() ) / ( RAND_MAX + 1 );

      x = x * math::pi12f / a;
      y = y * math::pi12f / a;
      z = z * math::pi12f / a;

      auto zs = sin( z ), zc = cos( z );
      auto s = sin( x ), c = cos( x );

      //auto q = normalize( math::quat< float >{ x, y, z, 0 } );
      auto q = normalize( math::quat< float >{ zc*c, zc*s, zs, 0.025f * ww } );

      q[ index ] = q[ index ] + ( 0.5*rnd1 - 0.25f );

      range{ 0, iter } $do { 

        if( mul == 1 ) q = qmul1( q, q );
        if( mul == 2 ) q = qmul2( q, q );
        if( mul == 3 ) q = qmul3( q, q );
        if( mul == 4 ) q = qmul4( q, q );

        auto r = rot * vec3f{ q[0], q[1], q[2] };
        
        r = 20.f * r + vec3f{.0f,.0f,40.f};
        if( r[2] > 1.f ) r = r / vec3f{ r[2] };

        int u = ( r[ 0 ] + 1.f ) / 2. * w;
        int v = ( r[ 1 ] + 1.f ) / 2. * h;

        u = lib::max( 0, lib::min( u, w-1 ));
        v = lib::max( 0, lib::min( v, h-1 ));

        auto c = data[ v ][ u ];

        if( not ( ( c & 0xFF ) == 255 or 
                  ( c >> 8 & 0xFF ) == 255 or 
                  ( c >> 16 & 0xFF) == 255 ) )

          data[ v ][ u ] += 0x00'01'01'01;
      };
    }

  wnd.set_data( (uint*) data );
}


int main() {

  //lib::log::memory.on();
  //lib::log::lock.on();
  lib::log::link.on();
  lib::log::event.on();
  //lib::log::input.on();


  info, "start program", endl;

  $event( "key_up" ) {

    if( event.key == os::vkey::_1 ) { mul = 1; info, "qmul = ", mul, endl; }
    if( event.key == os::vkey::_2 ) { mul = 2; info, "qmul = ", mul, endl; }
    if( event.key == os::vkey::_3 ) { mul = 3; info, "qmul = ", mul, endl; }
    if( event.key == os::vkey::_4 ) { mul = 4; info, "qmul = ", mul, endl; }
    
    if( event.key == os::vkey::space ) { rot = mat3f{ 1.f }; }

    if( event.key == os::vkey::tab ) {
      drawing = drawing != 1;
      if( drawing ) info, "drawing", endl;
      else info, "not drawing", endl;
    }

    data_clear();

    if( event.key == os::vkey::escape ) {
      info, "got escape, exiting", endl;
      loop = false;
    }
    return true;
  };

  auto wnd0 = os::window::create( "Test Window", w, h );

  $event( "scroll" ) {
    ww = ww + event.y;
    info, "ww = ", ww, endl;
    data_clear();
    return true;
  };

  $event( "mouse_move" ) {

    if( rotate ) {  
      auto o = vec2i{ event.x, event.y }; 
      auto v = o - xy_prev;
      xy_prev = o;

      if( v[0] != 0 ) rot = dot( roty[ v[0] < 0 ? 0 : 1 ], rot );
      if( v[1] != 0 ) rot = dot( rotx[ v[1] < 0 ? 0 : 1 ], rot );

      rot = normalize( rot );

      data_clear();
    }

    return true;
  };

  $event( "mouse_down" ) {

    xy_prev = {event.x, event.y};
    rotate = true;
    iter = 8;

    return true;
  };

  $event( "mouse_up" ) {

    rotate = false;

    if( event.key == os::vkey::rbutton ) {
      ww = 0;
      index = ( index + 1 ) % 4;
      info, "index = ", index, endl;
   }

    iter = 15;

    data_clear();

    return true;
  };


  lib::global::event_map<>.dump_events();

  os::input in;

  while( loop ) {

    if( not in() ) break;

    if( drawing ) draw( wnd0 );

    Sleep( 0 );
  }

}


