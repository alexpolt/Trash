#ifndef alexpolt_iterator
#  define alexpolt_iterator

#include "../config.h"

namespace alexpolt {

  /*
    Simple helper iterator
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  template<class T>
  struct array_iterator {
    T* pos_m;

    explicit array_iterator( T* array ) : pos_m { array } {}

    T* operator*() { return pos_m; }
    bool operator!=( array_iterator r ) { return pos_m != r.pos_m; }
    T* operator++() { return ++pos_m; }
  };

  template<class T>
  array_iterator<T> make_array_iterator( T* array ) {
    return array_iterator<T>{ array };
  }

}

#endif //alexpolt_iterator
