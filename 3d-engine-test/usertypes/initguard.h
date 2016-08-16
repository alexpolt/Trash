#ifndef alexpolt_initguard
#  define alexpolt_initguard

#include "../config.h"

namespace alexpolt {

  /*
    Used to call namespace::end() on destruction in main file.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

struct initguard {
  bool success_m;
  using endfunction = void(*)();
  endfunction endfunc_m;

  initguard( bool success, endfunction endfunc = nullptr ) : success_m{ success }, endfunc_m { endfunc } {}

  operator bool() { return success_m; }

  delete_copy_assign( initguard )
  default_copy_assign_move( initguard )

};


}

#endif //alexpolt_initguard

