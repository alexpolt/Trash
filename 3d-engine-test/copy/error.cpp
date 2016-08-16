#include "config.h"
#include "usertypes/cstring.h"
#include "usertypes/string.h"
#include "error.h"

namespace alexpolt { namespace error {

  /*
    Error handling.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  TLS code errorcode{ error::code::failed };
  TLS cstring errorcstring { cemptystring };
  TLS char errorstring[ sizeof (string) ] {};

  //TLS string errorstring; //problems with TLS emulation in GCC - generates non TLS symbol in setError

  struct init {
    init() {}
  } init_guard;

}}

