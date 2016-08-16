
#include <cstdio>
#include <cmath>
#include <complex>

#include "../window.h"
//#include "../error.h"
#include "../image/bmp.h"

#include "../../polynom.h"

using namespace alexpolt;


template<class T>
struct cb : ui::onkeycallback {
  T op_m;
  cb( T op ) : op_m{ op } {}
  void key( const window& wnd, WPARAM wparam ) override {
    if( wparam == VK_SPACE ) op_m( wnd, wparam );
  }
};

using C = std::complex<float>;

C c0{}, c1{1.0f}, c11{1.5f, -2.0f}, c12{-1.0f, 1.5f};

Polynom<C> poly{ c1, c0, c1, c0, c1 };


int CALLBACK WinMain( HINSTANCE hInst, HINSTANCE hprevInst, LPSTR cmdline, INT nCmdShow ) {

  ui::begin();

  window wnd1 = make_fullscreen_window( );
  wnd1.show();

  image::bmp bmp1 = make_object<image::bmp>( 800, 600 );
  float w = bmp1.width(); 
  float h = bmp1.height();
  float step{};

  auto render = [w, h, &step]( int x_, int y_, uint32_t color ) -> uint32_t {
    float xx = x_- w/2, yy = y_- h/2;
    xx = xx / w * 4.0f;
    yy = yy / h * 4.0f;

    poly.data[2] = complex<float>{ 2*cosf( step )*cosf( step ), 2*cosf( step )*sinf( step ) };

    complex<float> c = poly( complex<float>{ xx, yy } ); 
    float newcolor = std::abs( c ) * 65536.0f / 16.0f;
    return uint32_t( newcolor );
    /*
    xx = .5f + ( xx / w );
    yy = .5f + ( yy / h );
    const float PI = 3.141592;
    return 255.0f * ( 1.0f + cosf( step * xx * PI * 5 + yy * yy * PI * 8 )) / 2.0f;
    */
  };

  auto paint = [w,h,&step,&render,&bmp1]( const window& wnd, WPARAM wparam ) -> void {
    step+=0.1f;
    bmp1.render( render );
    ok ok_ = bmp1.setdibits ( wnd );
    log_result( ok_ );
  };

  cb< decltype(paint) > keycb{ paint };
  wnd1.onkeyup( &keycb );
  
  
  MSG msg; BOOL ret;

  do {
    ret = GetMessage( &msg, NULL, 0, 0 );

    if( ( ret == 0 || ret == -1 ) 
          && ! ui::hwnd_list.size() )
            break;
    TranslateMessage( &msg );
    DispatchMessage( &msg );
    //if(! IsDialogMessage( NULL, &msg ) ) {
    //if( WM_DESTROY == msg.message )

  } while( 1 );

  printf("exit\n");
  getchar();

return 0;
}


