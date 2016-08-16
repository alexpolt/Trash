#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <windows.h>
#include <cmath>
#include <cstdlib>
#include "polynom.h"
#include <complex>

template<class T>
struct memdata final {
  memdata(size_t sz) : size(sz) {
    data = (T*) malloc( size );
  }
  memdata(const memdata<T>& r) = delete;
  memdata& operator=(const memdata&) = delete;
  ~memdata() { free(data); }

  T* operator->() { return data; }
  T& operator[]( ptrdiff_t off ) { if( off * sizeof( T ) < size ) return data[ off ]; return *reinterpret_cast<T*>( 0 ); }

  T* data;
  size_t size;
};

template<class T>
void renderImage( int w, int h, memdata<uint32_t>&, T );

int maxwalks, walks;
double maxcolor_g{};

using C = std::complex<double>;
C c0{}, c1{1.0f}, c11{1.5f, -2.0f}, c12{-1.0f, 1.5f};
Polynom<C> poly{ c1, c0, c1, c0, c1 };
//Polynom<C> poly{ c0, c12, c0, c12, c1, c0, c11 };

int main() {

  const int w = 1024, h = 768, imgSize = w * h * sizeof( uint32_t );
  
  int32_t imgDataOffset = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFO ) - sizeof( RGBQUAD );
  BITMAPFILEHEADER bfh = { 'M' << 8 | 'B', DWORD( imgDataOffset + imgSize ), 0, 0, DWORD( imgDataOffset ) };
  BITMAPINFO bmi = {};
  BITMAPINFOHEADER* bi = &bmi.bmiHeader;
  bi->biSize = sizeof( BITMAPINFOHEADER );
  bi->biWidth = w;
  bi->biHeight = -h;
  bi->biPlanes = 1;
  bi->biBitCount = 32;
  bi->biSizeImage = imgSize;
  bi->biCompression = BI_RGB;
  bi->biXPelsPerMeter = 0;
  bi->biYPelsPerMeter = 0;
  bi->biClrUsed = 0;
  bi->biClrImportant = 0;
  
  memdata<uint32_t> data{ w * h * sizeof( uint32_t ) };

  auto func = [w,h]( int x_, int y_, uint32_t color ) -> uint32_t {
    double x = x_- w/2, y = y_- h/2;
    x = x / w;
    y = y / h;
    const double PI = 3.141592f; 
  
    complex<double> c = poly( complex<double>{ 3.0f * x, 3.0f * y } ); 
    double newcolor = std::abs( c ) * 65536.0f / 16.0f;
	//maxcolor_g = maxcolor_g < newcolor && newcolor < 4.0e6 ? newcolor : maxcolor_g;
    return uint32_t( newcolor );
    //return uint32_t( r ) | uint32_t( g ) | uint32_t( b );
    
/*
    uint32_t color = 0;
    double a = cos(  (x*x+1.0f) * PI * 10.0f  + (y*y*y+1.0f) * PI * 4.0f  );
    color = round( 127.0f * ( 1.0f + a ));
    return color;
*/

/*
    uint32_t color = 0;

    struct { double x,y,z,r; } spheres[] = { {-10.0f, -6.0f, 40.0f, 6.0f }, {2.0f, -.5f, 4.0f, 1.2f }, {0.5f, .4f, 2.0f, 0.3f } };
    y = y * 3.0f/8.0f;
    x = x / 2.0f;
    struct { double x,y,z; } view = { x / sqrtf( x*x + y*y + 1 ), y / sqrtf( x*x + y*y +1 ), 1/sqrtf( x*x + y*y + 1 ) };
    for( auto& sp : spheres ) {
      double proj = sp.x * view.x + sp.y * view.y + sp.z * view.z;
      double dist = ( sp.x - view.x * proj ) * ( sp.x - view.x * proj ) + ( sp.y - view.y * proj ) * ( sp.y - view.y * proj ) + ( sp.z - view.z * proj ) * ( sp.z - view.z * proj ) ;
      if( dist < sp.r * sp.r ) {
        double err = 1;
        double distsp = ( view.x * proj ) * ( view.x * proj ) + ( view.y * proj ) * ( view.y * proj ) + ( view.z * proj ) * ( view.z * proj ) ;
        dist = distsp;
        int walks = 0;
        while( err > 0.5 ) {
          distsp = distsp / 2;
          if( err < 0 ) dist += distsp;
          else dist -= distsp;
          err = dist - sp.r * sp.r;
          if( maxwalks < ++walks ) maxwalks = walks;
        }
        double NxV = fabs( view.x * ( sp.x - view.x * dist ) + view.y * ( sp.y - view.y * dist ) + view.z * ( sp.z - view.z * dist ) );
        color = 255.0f * ( NxV - round( NxV ) );
        if( color == 0 ) color = 255;
        else color &= 0xFF;
      }
      color = color << 16 | color << 8 | color;
    }
    return color;
*/
  
  };
  auto func2 = [w, h]( int x_, int y_, uint32_t color ) -> uint32_t {
	return double( color ) / maxcolor_g * 255.0f;
  };

  renderImage( w, h, data, func );
  //renderImage( w, h, data, func2 );
  
  FILE* file = fopen("bmp1.bmp", "w+b");
  if( file == nullptr ) {
    perror("bmp1 file fopen error\n");
  } else {
    fwrite( &bfh, 1, sizeof( bfh ), file );
    fwrite( &bmi, 1, sizeof( bmi ) - sizeof( RGBQUAD ), file );
    fwrite( &data[0], 1, data.size, file );
    fclose( file );
  }
  
  printf("maxwalks = %d\n", maxwalks);
  
return 0;
}

template<class T>
void renderImage( int w, int h, memdata<uint32_t>& data, T func ) {
  ptrdiff_t offset;

  for(size_t y = 0; y < h; y++ ) {
    for(size_t x = 0; x < w; x++ ) {
        offset = y * w + x;
        data[ offset ] = func( x, y, data[ offset ] );
    }
  }
}




