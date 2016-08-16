#include <cstdio>

#include "config.h"
#include "usertypes/cstring.h"
#include "usertypes/string.h"
#include "logging.h"
#include "error.h"

namespace alexpolt { namespace logging {

  /*
    Logging facilities.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

cstring logtype2cstring[] = { "Message"_str, "Warning"_str, "Error"_str };

void log( logtype logtype, cstring message ) {
    printf( "%s: %s\n", logtype2cstring[ (size_t) logtype ].c_str(), message.c_str() );
}

void log( logtype logtype, const string& message ) {
    printf( "%s: %s\n", logtype2cstring[ (size_t) logtype ].c_str(), message.c_str() );
}

void logError() {
  if( error::errorcstring.empty() ) {
    log( logging::logtype::message, aserrorstring( error::errorstring )  );
  } else
    log( logging::logtype::message, error::errorcstring );
}


}}


