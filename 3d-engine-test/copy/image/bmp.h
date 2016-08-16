#ifndef alexpolt_imagebmp
#  define alexpolt_imagebmp

#include <cstring>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdint>

#include "../config.h"
#include "../common.h"


namespace alexpolt { namespace image {

  /*
    BMP image format handling.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */


template<class T>
struct memdata final {
  T* data_m;
  size_t size_m;

  memdata() : data_m{}, size_m{} {}

  explicit memdata(size_t sz) : size_m(sz) {
    data_m = heap::malloc<T>( sz );
  }
  memdata( memdata&& r ) : data_m{ r.data_m }, size_m{ r.size_m } {
    r.data_m = nullptr; r.size_m = 0;
  }
  memdata& operator=( memdata&& r ) {
    if( data_m != nullptr ) heap::free( data_m );
    data_m = r.data_m; size_m = r.size_m;
    r.data_m = nullptr; r.size_m = 0;
    return *this;
  }

  delete_copy_assign( memdata<T> )

  ~memdata() { 
    if( data_m ) heap::free( data_m );
   }

  T* operator->() { return data_m; }

  T& operator[]( ptrdiff_t off ) { 
    if( off * sizeof( T ) < size_m ) return data_m[ off ]; return *reinterpret_cast<T*>( 0 ); 
  }

  size_t size() const { return size_m; }
  T* data() const { return data_m; }

};

struct bmp : object {
  struct bmpinfo {
    BITMAPFILEHEADER bfh;
    BITMAPINFO bmi;
    memdata<uint32_t> data;
    size_t width, height;
  } *info;

  static const size_t img_data_offset = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFO ) - sizeof( RGBQUAD );
  static const size_t defwidth = 1024, defheight = 768;

  bmp () = default;
  ~bmp () { destruct(); }

  bmp(bmp&& r) : info{ r.info } {
    r.info = nullptr;
  }
  bmp& operator=(bmp&& r) = delete;

  delete_copy_assign( bmp );

  void construct( size_t width_ = defwidth, size_t height_ = defheight ) {
    info = make_object_heap<bmpinfo>();

    info->width = width_;
    info->height = height_;
    size_t img_size = width_ * height_ * sizeof( uint32_t );

    BITMAPFILEHEADER bfh_ = { 'M' << 8 | 'B', DWORD( img_data_offset + img_size ), 0, 0, DWORD( img_data_offset ) };
    info->bfh = bfh_;

    BITMAPINFO bmi_ = {};
    bmi_.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
    bmi_.bmiHeader.biWidth = width_;
    bmi_.bmiHeader.biHeight = -height_;
    bmi_.bmiHeader.biPlanes = 1;
    bmi_.bmiHeader.biBitCount = 32;
    bmi_.bmiHeader.biSizeImage = img_size;
    bmi_.bmiHeader.biCompression = BI_RGB;
    bmi_.bmiHeader.biXPelsPerMeter = 0;
    bmi_.bmiHeader.biYPelsPerMeter = 0;
    bmi_.bmiHeader.biClrUsed = 0;
    bmi_.bmiHeader.biClrImportant = 0;
    info->bmi = bmi_;

    info->data = memdata<uint32_t>{ width_ * height_ * sizeof( uint32_t ) };
  }

  void destruct() {
    if( info != nullptr ) 
      destroy_object_heap( info );
  }

  template<class T>
  void render( T func ) {
    ptrdiff_t offset;
    for(size_t y = 0; y < info->height; y++ ) {
      for(size_t x = 0; x < info->width; x++ ) {
          offset = y * info->width + x;
          info->data[ offset ] = func( x, y, info->data[ offset ] );
      }
    }
  }

  ok save( string filename ) {
    FILE* file = fopen( filename.c_str(), "w+b");
    if( file == nullptr ) {
      return make_notok( string{ strerror( errno ) } );
    } else {
      fwrite( &info->bfh, 1, sizeof( info->bfh ), file );
      fwrite( &info->bmi, 1, sizeof( info->bmi ) - sizeof( RGBQUAD ), file );
      fwrite( info->data.data(), 1, info->data.size(), file );
      fclose( file );
    }
    return make_ok();
  }

  ok setdibits( const ui::window& wnd ) {
    HDC hdc = wnd.dc();
    HWND hwnd = wnd.hwnd();

    ok ok_ = make_ok();

    HBITMAP bitmap = CreateCompatibleBitmap( hdc, info->width, info->height );
    if( !bitmap ) { 
      ReleaseDC( hwnd, hdc );
      return make_notok( windows::getLastErrorString() );
    }

    HDC memdc = CreateCompatibleDC( hdc );
    HBITMAP oldbitmap = (HBITMAP)SelectObject( memdc, bitmap );

    int numlines = SetDIBits( memdc, bitmap, 0, info->height, &info->data[0], &info->bmi, DIB_RGB_COLORS );
    if( !numlines ) 
      ok_ = make_notok( windows::getLastErrorString() );

    if( !BitBlt( hdc, 0, 0, wnd.width(), wnd.height(), memdc, 0, 0, SRCCOPY ) ) {
      ok_ = make_notok( windows::getLastErrorString() );
    }

    SelectObject( hdc, oldbitmap );
    ReleaseDC( hwnd, hdc );
    DeleteObject( bitmap );
    DeleteDC( memdc );

    return ok_;
  }

  size_t width() const { return info->width; }
  size_t height() const { return info->height; }

};

}} 
  


#endif //alexpolt_imagebmp


