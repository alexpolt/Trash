#ifndef alexpolt_make
#  define alexpolt_make

#include "config.h"
#include "util.h"
#include "heap.h"
#include "types/object.h"

namespace alexpolt { namespace make {

 /*
   Object construction.

   Alex Poltavsky, 2013
   alexpolt@yandex.ru
 */


  template<class T, class... Args> 
    T make_object_( int, Args&&... args ) {
        return T { util::forward<Args>( args )... };
    }

  template<class T, class... Args> 
    T make_object_( char, Args&&... args ) {
        T object{ };
        object.construct( util::forward<Args>( args )... );
        return util::move(  object  );
    }

  template<class T, class... Args> 
    T make_object( Args&&... args ) {
      using overload = typename util::conditional< util::is_base_of<object, T>::value, char, int >::type;
      return make_object_<T>( overload{}, util::forward<Args>( args )... );
    }


  template<int N> struct make_object_heap_;
  template<> struct make_object_heap_<0> {
    template<class T, class... Args> static T* make_object_heap( Args&&... args ) {
        return new ( heap::malloc<T>( ) )  T{ util::forward<Args>( args )... };
    }
  };

  template<> struct make_object_heap_<1> {
    template<class T, class... Args> static T* make_object_heap( Args&&... args ) {
        T* value = new ( heap::malloc<T>() ) T{};
        value->construct(  util::forward<Args>( args )...  );
        return value;
    }
  };

  template<class T, class... Args> 
    T* make_object_heap( Args&&... args ) {
        return make_object_heap_< util::is_base_of<object, T>::value >::template make_object_heap<T>( util::forward<Args>( args )... );
    }
  
  template<class T> 
  void destroy_object_heap( T* object ) {
    object->~T( );
    heap::free( object );
  }

}

using make::make_object;
using make::make_object_heap;
using make::destroy_object_heap;


} 


#endif // alexpolt_make


