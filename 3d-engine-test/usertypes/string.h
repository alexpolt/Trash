#ifndef alexpolt_string
#  define alexpolt_string

#include <cstring> //strlen
#include <new>

#include "../config.h"
#include "../global.h"
#include "../heap.h"
#include "cstring.h"

namespace alexpolt {

  /*
    String type.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */


template<class T = char>
struct String {
    size_t size_m;
    T* data_m;

    String( const char* str, size_t size ) : size_m{ size }, data_m{ heap::malloc<T>( size ) } { 
      memcpy( data_m, str, size );
    }

    explicit String( const char* str ) : String{ str, strlen( str ) + 1 }  { }

    template<int N>
    explicit String( char ( &str ) [ N ]  ) : size_m{ N }, data_m{ heap::malloc<T>( size ) } { 
      memcpy( data_m, str, size );
    }

    String() : size_m{0}, data_m{nullptr} {}

    explicit String( size_t size ) : size_m{ size }, data_m{ heap::malloc<T>( size ) } { }

    explicit String( global::staticbuffer& tempbuffer ) : size_m{ tempbuffer.size() }, data_m{ tempbuffer.data() } { }

    String<T>( const cstring& str ) : String{ str.data(), str.size() } { }

    String<T>( const String<T>& rstr )  : String{ rstr.data_m, rstr.size_m } { }

    String( String<T>&& rstr ) : size_m{ rstr.size_m }, data_m{ rstr.data_m } { 
      rstr.size_m = 0; rstr.data_m = nullptr; 
    }

    String<T>& operator=( String<T> rstr ) {
      if( this == &rstr ) assert( !"Self-assigment" );
      this->~String<T>();
      new ( this ) String<T> { util::move( rstr ) };
      return *this;
    }

    ~String() { 
      if( data_m != global::tempbuffer.data() 
            && data_m != nullptr ) 
                heap::free<T>( data_m ); 
      data_m = nullptr;
      size_m = 0; 
    }

    size_t size() const { return size_m; }
    T* data() const { return data_m; }
    T* c_str() const { return data_m; }

    bool empty() const { return size == 0; }
};

using string = String<char>;

inline string make_string( const char* str, size_t size ) {
    return string{ str, size };
}

inline string make_string( global::staticbuffer& tempbuffer ) {
    return string{ tempbuffer };
}



}

#endif //alexpolt_String
