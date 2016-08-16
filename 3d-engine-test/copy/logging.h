#ifndef alexpolt_logging
#  define alexpolt_logging

#include "config.h"
#include "usertypes/cstring.h"
#include "usertypes/string.h"

namespace alexpolt { namespace logging {

  /*
    Logging facilities.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

enum class logtype : unsigned {
    message = 0, warning = 1, error = 2
};

void log( logtype logtype, cstring message );
void log( logtype logtype, const string& message );
void logError();

}}


#endif //alexpolt_logging

