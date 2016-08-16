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

#ifndef SKIP_NEW
#if defined ALLOW_NEW

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

#else //do not allow new in code

  void* operator new( size_t size ) noexcept;
  void* operator new[]( size_t ) noexcept;
  void operator delete( void* ptr ) noexcept;
  void operator delete[]( void* ptr ) noexcept  ;

#endif //end allow new

// placement new 
  inline void* operator new( size_t size, void* ptr ) noexcept { return ptr; }
  inline void* operator new[]( size_t, void* ptr ) noexcept { return ptr; }
  inline void operator delete( void* ptr, void*  ) noexcept { }
  inline void operator delete[]( void* ptr, void* ) noexcept { }


#endif //SKIP_NEW

#undef noexcept


#endif //alexpolt_newdelete

