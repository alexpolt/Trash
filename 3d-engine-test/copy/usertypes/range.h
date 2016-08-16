#ifndef alexpolt_range
#  define alexpolt_range

#include "../config.h"
#include "iterator.h"

namespace alexpolt {

  /*
    Small range (python like) type.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  template<class T = size_t> 
  struct range {
    T begin_m, end_m;

    range() = default;
    ~range() = default;

    delete_copy_assign( range )
    delete_copy_assign_move( range )

    range( T begin, T end ) : begin_m {begin}, end_m {end} { }

    template<class> friend struct array_iterator;
    array_iterator<T> begin() { return make_array_iterator( begin_m ); }  
    array_iterator<T> end() { return make_array_iterator( end_m ); }  

  };


}

#endif //alexpolt_range
