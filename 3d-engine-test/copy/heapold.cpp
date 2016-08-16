#define SKIP_NEW

#include "config.h"
#include "util.h"
#include "heap.h"
#include "result.h"
#include "usertypes/range.h"

#undef SKIP_NEW

#include <cstdio>
#include <vector>
#include <deque>
#include <set>
#include <malloc.h>

namespace alexpolt { namespace heap {

 /*
   Memory and object construction.
   Implementation.

   Alex Poltavsky, 2013
   alexpolt@yandex.ru
 */


  result::ok begin() {
    return result::make_ok();
  }

  result::ok end() {
    return result::make_ok();
  }


  std::vector<void(*)()> memprint{};

  std::set<void*> malloced{};

  struct malloced_guard { 
    static const int initsize = 256;
    malloced_guard() { }
    ~malloced_guard() {
        if( malloced.size() > 0 ) {
           printf( "Leaked memory! Size of malloced = %d\n", malloced.size() );
           for( auto ptr : malloced ) { printf( "free( %p )\n", ptr ); ::free( ptr ); }
        }
    }
  } malloced_guard_;


  template<class U> struct memory_guard;

  template<class T> struct memory {
    using index = unsigned;
    static const int initsize = 256;

    static std::deque<T> list;
    static std::vector<T*> free;
    static memory_guard<T> guard;

    static void print();
  };

  template<class T>
  struct memory_guard { 
     memory_guard() { 
        using memory = memory<T>;
        memory::free.reserve( memory::initsize );
        memprint.push_back( memory::print );
     } 
    ~memory_guard() { }
  };

  template<class T> std::deque<T> memory<T>::list{};
  template<class T> std::vector<T*> memory<T>::free{};
  template<class T> memory_guard<T> memory<T>::guard{};

  template<class T> T* malloc_( size_t size ) {
      using memory = memory<T>;
      (void) memory::guard;

      if( size > 1 ) { //standard malloc way for arrays
          T* ptr = (T*) ::malloc( sizeof( T ) * size );
          malloced.insert( ptr );
          return ptr;
      }

      if( memory::free.size() > 0 ) {
          auto ptr = memory::free.back();
          memory::free.pop_back();
          return ptr;
      }

      memory::list.push_back( T{} );
      return &memory::list.back();
  }

  template<class T> void free_( T* ptr ) {
      using memory = memory<T>;

      auto ptrit = malloced.find( ptr );

      if(  ptrit != malloced.end() ) {
        ::free( ptr );
        malloced.erase( ptrit );
      } else {
        memory::free.push_back( ptr );
      }
  }
  
  void print() {
    for( auto printfunc : memprint ) printfunc();

    printf( "Malloced memory, size = %d, ( ", malloced.size() );
    for( auto ptr : malloced ) printf( "0x%p ", ptr );

    printf( ")\n\n" );
  }

  template<class T> void memory<T>::print() {
      using memory = memory<T>;

      printf( "list<sizeof(%d)>, size = %d, ( ", sizeof( T ), memory::list.size() );
      for( const auto& i : memory::list )  printf( "%x ", * (int*) &i );

      printf( ")\n" );

      printf( "free<sizeof(%d)>, size = %d, ( ", sizeof( T ), memory::free.size() );
      for( const auto& i : memory::free ) printf( "%x ", * (int*) &i );

      printf( ")\n" );
  }
  

  //instantiate malloc_ and free_ for different arguments

  template<class... T> struct inst_malloc_type;
  template<class H, class... T> struct inst_malloc_type<H, T...> : inst_malloc_type<T...> {
      decltype( &malloc_<H> ) malloc = malloc_<H>;
      decltype( &free_<H> ) free = free_<H>;
  };
  template<> struct inst_malloc_type<> {};
  inst_malloc_type<char> instmalloc1; // instanciate for char type

  template<int S, int E> struct inst_malloc_size : inst_malloc_size<S+1, E> {
      decltype( &malloc_<data<S>> ) malloc = malloc_<data<S>>;
      decltype( &free_<data<S>> ) free = free_<data<S>>;
  };
  template<int S> struct inst_malloc_size<S, S> {};
  inst_malloc_size<1, 4> instmalloc2; // instanciate for sizes 1..4


}} 


