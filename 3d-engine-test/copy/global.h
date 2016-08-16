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
  struct globalbuffer {
    size_t size_m = N;
    char data_m[ N ]; //scratch buffer

    globalbuffer<N>() = default;
    ~globalbuffer<N>() = default;

    delete_copy_assign( globalbuffer<N> )
    delete_copy_assign_move( globalbuffer<N> )

    size_t size() { return size_m; }
    char* c_str() { return data_m; }
    char* data() { return data_m; }
  };

  using buffer = globalbuffer<1024>;

  extern buffer tempbuffer;

}}


#endif //alexpolt_global

