#ifndef alexpolt_heap
#  define alexpolt_heap

#include "config.h"

namespace alexpolt { namespace heap {

 /*
   Memory handling.

   Alex Poltavsky, 2013
   alexpolt@yandex.ru
 */

  void begin();
  void end();

  void* malloc_( size_t );
  void  free_( void* );

  template<class T> T* malloc( size_t size = 1 ) {
    assert( size != 0 && "heap::malloc( 0 )" );
    auto ptr = malloc_( sizeof( T ) * size  );
    return reinterpret_cast<T*>(  ptr  );
  }

  template<class T> void free( T* ptr ) {
    free_( ptr );
  }

#ifdef ALXP_DEBUG
  void print();
#endif


}


} 


#endif // alexpolt_heap


