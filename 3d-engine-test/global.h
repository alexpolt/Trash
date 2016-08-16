#ifndef alexpolt_global
#  define alexpolt_global

#include "config.h"

namespace alexpolt { namespace global {

  /*
    Global data.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  template<size_t N>
  struct staticbuffer_ {
    size_t size_m = N;
    char data_m[ N ]; //scratch buffer

    staticbuffer_<N>() = default;
    ~staticbuffer_<N>() = default;

    delete_copy_assign( staticbuffer_<N> )
    delete_copy_assign_move( staticbuffer_<N> )

    size_t size() { return size_m; }
    char* c_str() { return data_m; }
    char* data() { return data_m; }
  };

  using staticbuffer = staticbuffer_<1024>;

  extern staticbuffer tempbuffer;

}}


#endif //alexpolt_global

