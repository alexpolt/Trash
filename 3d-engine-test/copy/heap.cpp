#define SKIP_NEW

#include "config.h"
#include "heap.h"

#undef SKIP_NEW

#include <cstdio>
#include <set>
#include <malloc.h>

namespace alexpolt { namespace heap {

 /*
   Memory handling.
   Implementation.

   Alex Poltavsky, 2013
   alexpolt@yandex.ru
 */

  void begin() {
  }

  void end() {
  }

  std::set<void*> malloced{};

  struct malloced_guard { 
    static const int initsize = 256;

    malloced_guard() { }

    ~malloced_guard() {
        if( malloced.size() > 0 ) {
#         ifdef  ALXP_DEBUG
           printf( "Leaked memory! Size of malloced = %d\n", malloced.size() );
#         endif
           for( auto ptr : malloced ) { printf( "free( %p )\n", ptr ); ::free( ptr ); }
        }
    }

  } malloced_guard_;


  void* malloc_( size_t size ) {
          void* ptr = ::malloc( size );
          malloced.insert( ptr );
          return ptr;
  }

  void free_( void* ptr ) {
      auto ptrit = malloced.find( ptr );
      if(  ptrit != malloced.end() ) {
        ::free( ptr );
        malloced.erase( ptrit );
      } else {
         assert( !"_free( ptr ) Pointer not found!\n" );
      }
  }
  
  void print( ) {
    printf( "Malloced memory, size = %d, ( ", malloced.size() );
    for( auto ptr : malloced ) printf( "0x%p ", ptr );
    printf( ")\n\n" );
  }



}} 


