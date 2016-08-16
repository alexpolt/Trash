
#include <cstdio>
#include <cmath>

#include "../window.h"
#include "../error.h"
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

int CALLBACK WinMain( HINSTANCE hInst, HINSTANCE hprevInst, LPSTR cmdline, INT nCmdShow ) {

  ui::begin();

  window wnd1 = make_default_window( "Test window 1"_str );  
  window wnd2 = make_default_window( "Test window 2"_str );  

  wnd1.show();
  wnd2.show();

  image::bmp bmp1 = make_object<image::bmp>();
  float w = bmp1.width(); 
  float h = bmp1.height();
  int step{}; step++;

  auto render = [w, h, &step]( int x_, int y_, uint32_t color ) -> uint32_t {
    float xx = x_- w/2, yy = y_- h/2;
    xx = .5f + ( xx / w );
    yy = .5f + ( yy / h );
    const float PI = 3.141592;
    return 255.0f * ( 1.0f + cosf( step * xx * PI * 5 + yy * yy * PI * 8 )) / 2.0f;
  };

  auto paint = [w,h,&step,&render,&bmp1]( const window& wnd, WPARAM wparam ) -> void {
    step++;
    bmp1.render( render );
    ok ok_ = bmp1.setdibits ( wnd );
    ok_.log();
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


