#ifndef alexpolt_usertypes
#  define alexpolt_usertypes

#include "config.h"
#include "heap.h"
#include "result.h"

namespace alexpolt {

  /*
    Basically literal types and some helper user-defined literals.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

struct cstring {
    const char* data_m;
    size_t size_m;

    default_copy_assign( cstring );
    default_copy_assign_move( cstring );

    cstring() = default;
  
    cstring( const char* str, size_t size ) : data_m{ str }, size_m{ size } {}
    
    ~cstring() = default;

    size_t size() const { return size_m; }
    const char* data() const { return data_m; }
};

template<class T = char*>
struct string {
    T data_m;
    size_t size_m;

    string( const char* str, size_t size ) : size_m{ size } { 
      //copy string from str to data
    }

    template<int N>
    explicit string( char ( &str ) [ N ]  ) : size_m{ N } { 
      //copy string from str to data
    }

    explicit string( size_t size ) : size_m{ size }, data_m{ heap::malloc<T>( size ) } { }

    explicit string<T>( const cstring& str ) : string{ str.data(), str.size() } { }

    string<T>( const string<T>& str )  : string{ str.data_m, str.size_m } { }

    string( string<T>&& str ) : size_m{ str.size_m }, data_m{ str.data_m } { 
      str.size_m = 0; str.data_m = nullptr; 
    }

    ~string() { heap::free<T>( data_m ); data_m = nullptr; size_m = 0; }

    size_t size() const { return size_m; }
    T data() const { return data_m; }
};

cstring operator"" _str( const char* str, unsigned size ) { return cstring{ str, size }; }

template<class T>
string<T> make_string( T str, size_t size ) {
    return heap::CreateObject<string<T>>( str, size );
}


}

#endif //alexpolt_usertypes
