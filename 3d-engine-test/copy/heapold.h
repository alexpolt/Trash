#ifndef alexpolt_heap
#  define alexpolt_heap

#include "config.h"
#include "util.h"
#include "types/object.h"


namespace alexpolt { namespace heap {

 /*
   Memory and object construction.

   Alex Poltavsky, 2013
   alexpolt@yandex.ru
 */

  template<class T> T* malloc( size_t size = 1 );
  template<class T> void free( T* ptr );

  template<int T> struct CreateObject_;
  template<> struct CreateObject_<0>;
  template<> struct CreateObject_<1>;

  
  template<class T, class... Args> 
    T* CreateObject( Args&&... args ) {
        return CreateObject_< util::is_base_of<Object, T>::value >::template CreateObject<T>( util::forward<Args>( args )... );
    }


  template<> struct CreateObject_<0> {

    template<class T, class... Args> static T* CreateObject( Args&&... args ) {
        T* value = new ( malloc<T>( ) )  T{ util::forward<Args>( args )... };
        return value;
    }

  };

  template<> struct CreateObject_<1> {

    template<class T, class... Args> static T* CreateObject( Args&&... args ) {
        T* value = new ( malloc<T>() ) T{};
        value->construct( util::forward<Args>( args )... );
        return value;
    }

  };

  template<class T> T* malloc_( size_t ); // heap.cpp
  template<class T> void free_( T* ptr ); // heap.cpp

  //dummy data object
  template<size_t N> struct data { 
    unsigned data_m[ N ]; 
    data() {}
    ~data() {}
    delete_copy_assign( data );
    default_copy_assign_move( data );
  };

  template<class T> T* malloc( size_t size ) {
    using data = data< ( sizeof( T ) + sizeof(unsigned) - 1 ) / sizeof( unsigned ) >;
#  ifdef ALXP_DEBUG
    printf( "*%s( %d ), sizeof( T ) = %d\n" , __func__, size, sizeof( T ) );
#  endif
    if( size > 1 ) return reinterpret_cast<T*>(  malloc_<char>( sizeof( T ) * size )  );
    return reinterpret_cast<T*>( malloc_<data>( size ) );
  }

  template<class T> void free( T* ptr ) {
    using data = data< ( sizeof( T ) + sizeof(unsigned) - 1 ) / sizeof( unsigned ) >;
#  ifdef ALXP_DEBUG
    printf( "*%s( 0x%p ), sizeof(T) = %d\n", __func__,  ptr, sizeof( T ) );
#  endif
    free_<data>( reinterpret_cast<data*>( ptr ) );
  }

#ifdef ALXP_DEBUG
  extern void print();
#endif


}

using heap::CreateObject;

} 


#endif // alexpolt_heap


