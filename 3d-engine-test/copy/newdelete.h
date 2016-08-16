#ifndef alexpolt_newdelete
#  define alexpolt_newdelete

#include "heap.h"
#include "util.h"


namespace alexpolt { namespace newdelete {

  /*
    Overload global allocation operators.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

}}

using namespace alexpolt;

#define noexcept noexcept( true )

#ifdef ALLOW_NEW

  inline void* operator new( size_t size ) noexcept {
    return heap::malloc<char>( size );
  }

  inline void* operator new[]( size_t size ) noexcept {
    return heap::malloc<char>( size );
  }

  inline void operator delete( void* ptr ) noexcept {
    heap::free<char>( reinterpret_cast<char*>( ptr ) );
  }

  inline void operator delete[]( void* ptr ) noexcept {
    heap::free<char>( reinterpret_cast<char*>( ptr ) );
  }

#elif !defined SKIP_NEW  //no new

  void* operator new( size_t size ) noexcept;
  void* operator new[]( size_t ) noexcept;
  void operator delete( void* ptr ) noexcept;
  void operator delete[]( void* ptr ) noexcept  ;

#else    //skip new

#endif //end allow new


// placement new 
#if !defined SKIP_NEW  //no new

  inline void* operator new( size_t size, void* ptr ) noexcept { return ptr; }
  inline void* operator new[]( size_t, void* ptr ) noexcept { return ptr; }
  inline void operator delete( void* ptr, void*  ) noexcept { }
  inline void operator delete[]( void* ptr, void* ) noexcept { }

#endif

#undef noexcept


#endif //alexpolt_newdelete

