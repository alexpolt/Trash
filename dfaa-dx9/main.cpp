#include <cstdio>
#include <cfloat>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#include <mmsystem.h>
#include <sys/time.h>
#include <cassert>

#ifdef __clang__
struct __float128 {};
#endif

#include <type_traits>
#include <utility>


#define null 0
using hi_t = HINSTANCE;
using lpstr_t = LPSTR;
using cstr_t = const char*;
using cstr_ = const char*;
using hwnd_t = HWND;
using uint = unsigned;
using dword_t = DWORD;
using long_t = LONG;
using hr_t = HRESULT;
using ret_t = uint;

#define $appname "Amazing Cubes v1.0"
cstr_t appname = "Amazing Cubes";
uint   appver = 1 << 16;
cstr_t appversion = "1.0";
cstr_t appauthor = "Alexandr Poltavsky";
cstr_t appdate = "27-04-2015";

#define tp template
#define tn typename

#define $report_flag 1
#define $fileline_flag 1

#define $fl if( $fileline_flag ) { pf( "%s:%d\t", __FILE__, __LINE__ ); }
#define $fn if( $report_flag ) { $fl; p( __func__ ); }

#define $report( ... ) if( $report_flag ) { $fl; pf( __VA_ARGS__ ); p( "" ); }

#define $warn( expr ) if( ! expr ) { $fl; print_error( ); }
#define $error( expr ) if( ! expr ) { $fl; print_error( ); return null; }
#define $fatal( expr ) if( ! expr ) { $fl; print_error( ); exit( 1 ); }
#define $warn2( expr, msg ) if( ! expr ) { $fl; print_error( msg ); }
#define $error2( expr, msg ) if( ! expr ) { $fl; print_error( msg ); return null; }
#define $fatal2( expr, msg ) if( ! expr ) { $fl; print_error( msg ); exit( 1 ); }

#define $numargs_( _1, _2, _3, _4, _5, N, ... ) N
#define $numargs( ... ) $numargs_( __VA_ARGS__, 5, 4, 3, 2, 1 )

#define $check1( macro, VAR, _1 ) VAR = _1; macro( VAR, #_1 );
#define $check2( macro, VAR, _1, _2 ) $check1( macro, VAR, _1 ) $check1( macro, VAR, _2 )
#define $check3( macro, VAR, _1, _2, _3 ) $check2( macro, VAR, _1, _2 ) $check1( macro, VAR, _3 )
#define $check4( macro, VAR, _1, _2, _3, _4 ) $check3( macro, VAR, _1, _2, _3 ) $check1( macro, VAR, _4 )
#define $check5( macro, VAR, _1, _2, _3, _4, _5 ) $check4( macro, VAR, _1, _2, _3, _4 ) $check1( macro, VAR, _5 )
#define $check__( macro, VAR, N, ... ) $check##N( macro, VAR, __VA_ARGS__ )
#define $check_( macro, VAR, N, ... ) $check__( macro, VAR, N, __VA_ARGS__ )
#define $check( macro, VAR, ... ) $check_( macro, VAR, $numargs( __VA_ARGS__ ), __VA_ARGS__ )

#define $warnall( VAR, ... ) $check( $warn2, VAR, __VA_ARGS__ )
#define $errorall( VAR, ... ) $check( $error2, VAR, __VA_ARGS__ )
#define $fatalall( VAR, ... ) $check( $fatal2, VAR, __VA_ARGS__ )

#define $ret return
#define $ (*this)

inline void p( cstr_t msg ) { puts( msg ); }
tp<tn... ARGS> void pf( cstr_t format, ARGS... args ) { printf( format, args... ); }
void print_error( cstr_ = nullptr );

tp<tn A>
using noref = typename std::remove_reference<A>::type;

tp<tn T, int N> constexpr int size(T(&)[N]) { return N; }

tp<tn A> typename std::enable_if< !std::is_scalar< noref<A> >::value, noref<A> >::type&& mv(A&& arg) { 
  $ret static_cast<noref<A>&&>( arg );
}

tp<tn A> typename std::enable_if<std::is_scalar<noref<A>>::value,noref<A>>::type mv(A&& arg) { 
  noref<A> orig{arg}; arg=noref<A>{}; 
  $ret orig;
}

tp<tn A> 
struct com_obj_release {
  void operator()( A obj ) { if( obj ) obj->Release(); } 
};

tp<tn A, tn B> 
struct raii_t {
  A obj; B func; 

  raii_t() : obj{}, func{} {}
  raii_t( A obj, B func ) : obj(obj), func(func) { }
  raii_t(const raii_t&) = delete;
  raii_t(raii_t&& r) : obj(mv(r.obj)), func(mv(r.func)) { }
  ~raii_t() { func( obj ); }

  bool operator!() { return !obj; }
  operator A() { return obj; }
};

tp<tn A, tn B> 
raii_t<A, B> raii( A obj, B func ) { return raii_t<A, B>{ obj, func }; }

tp<tn A> 
struct comptr_t : raii_t< A*, com_obj_release<A*> > { 
  using base = raii_t< A*, com_obj_release<A*> >;
  comptr_t() {} 
  comptr_t( A* obj ) : base{ obj, com_obj_release<A*>{} } {} 
  A** operator &() {  return &$.obj; }
  A* operator->() { return $.obj; }
};

tp<tn A> 
comptr_t<A> comptr( A* obj ) { return comptr_t<A>{ obj }; }

LRESULT CALLBACK wndproc( hwnd_t, uint, WPARAM, LPARAM );
hwnd_t create_window( hi_t, long_t, long_t, WNDPROC );
void message_loop( hwnd_t );

int DFAA, pass2, pause, fps, zoom;

#include "directx.cpp"


int CALLBACK WinMain( hi_t hi, hi_t hprevi, lpstr_t cmdline, int cmdshow ) 
{

  p( $appname );

/*
  uint mxcsr{};
  _controlfp( _MCW_EM, _MCW_EM );
  _controlfp( _DN_FLUSH, _MCW_DN );
  _controlfp( _PC_53, _MCW_PC );
  _controlfp( _RC_NEAR, _MCW_RC );
  mxcsr = _controlfp( 0, 0 );
  $report("fp mxcsr %#0X, _mcw_em %#0X, fp _mcw_dn %#0X, fp _mcw_pc %#0X, _mcw_rc %#0X", 
            mxcsr, mxcsr & _MCW_EM, mxcsr & _DN_FLUSH, mxcsr & _PC_53, mxcsr & _RC_DOWN );
*/

  auto hwnd1 = raii( create_window( hi, 1344, 840, wndproc ), []( hwnd_t ){ $report("destroy window"); } );
  $fatal( hwnd1 );
  init_directx( hi, hwnd1 );
  timeBeginPeriod(1);
  message_loop( hwnd1 );
  timeEndPeriod(1);

  p( "done" );
  $ret 0; 
}

hwnd_t create_window( hi_t hi, long_t w, long_t h, WNDPROC proc ) {
  $fn;

  WNDCLASSEX wc{};
  wc.cbSize = sizeof ( wc );
  wc.style = CS_OWNDC;
  wc.lpfnWndProc = proc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hi;
  wc.hIcon = null;
  wc.hCursor = LoadCursor(  null, IDC_ARROW );
  wc.hbrBackground = null;
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = appname;
  wc.hIconSm = null;
  RegisterClassEx( &wc );

  //dword_t style = WS_OVERLAPPED | WS_SYSMENU;
  //dword_t style = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU;
  dword_t style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
  RECT rect{0,0,w,h}; 
  AdjustWindowRect( &rect, style, false );
  hwnd_t hwnd = CreateWindowEx( null, appname, $appname, style, CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, null, null, hi, nullptr );

  return hwnd;
}


void message_loop( hwnd_t hwnd ) {
  MSG msg;
  int mouse_x{}, mouse_y{};

  while( true ) {
    msg.message = 0;
    PeekMessage( &msg, null, 0, 0, PM_REMOVE );

    if( msg.message ) {
      //$report("MSG: %.X",msg.message);
      switch( msg.message ) {

        case WM_QUIT: 
            return;

        case WM_PAINT: 
            ValidateRect( hwnd, nullptr );
            break;

        case WM_LBUTTONUP: 
            break;

        case WM_KEYUP: 
            if( msg.wParam == VK_ESCAPE ) { DestroyWindow( hwnd ); return; }
            if( msg.wParam == VK_SPACE ) pause = 1 - pause;
            if( msg.wParam == 'A' ) { DFAA = DFAA ? false : true; $report("noaa = %d",DFAA); }
            if( msg.wParam == 'P' ) { pass2 = pass2 ? false : true; $report("pass2 = %d", pass2 ); }
            if( msg.wParam == 'F' ) { fps = fps ? false : true; }
            if( msg.wParam == 'Z' ) { zoom = zoom ? false : true; }
            break;

        case WM_MOUSEMOVE:
            mouse_x = GET_X_LPARAM( msg.lParam );
            mouse_y = GET_Y_LPARAM( msg.lParam );
        break;

        //case :
          //break;

        default: 
            DispatchMessage( &msg );
            continue;
      } //switch
    } //if

    //uint devider{};
    //if( ( ++devider % 5 ) == 0 );

    draw_loop( mouse_x, mouse_y );
    Sleep( 0 );

  } //while

}

LRESULT CALLBACK wndproc( hwnd_t hwnd, uint msg, WPARAM wparam, LPARAM lparam ) {

  switch( msg ) {

    case WM_CREATE: 
        break;
    case WM_CLOSE: 
        break;
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return null;
    //case :
      //break;
  }

return DefWindowProc( hwnd, msg, wparam, lparam );  
}

void print_error( cstr_ msg ) {
  if( msg ) p( msg );
  dword_t error = GetLastError();
  pf("win error %d ( %X )\n", error, error );
}


