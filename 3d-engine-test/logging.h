#ifndef alexpolt_logging
#  define alexpolt_logging

#include "config.h"
#include "usertypes/cstring.h"
#include "usertypes/string.h"

#define log( logtype, msg ) logging::log_message( logtype, msg, CC(__FILE__, _str), __LINE__ )
#define log_and_terminate( logtype, msg ) { logging::log_message( logtype, msg, CC(__FILE__, _str), __LINE__ ); assert( !"log and terminate" ); }

namespace alexpolt { namespace logging {

  /*
    Logging facilities.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

enum class log_type : unsigned {
    message = 0, warning = 1, error = 2
};

void log_message( log_type logtype, cstring message,  cstring file, int line);
void log_message( log_type logtype, const string& message,  cstring file, int line );
void log_error( cstring file, int line );

}

using logging::logtype;

}


#endif //alexpolt_logging

