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

void log_message( log_type logtype, cstring message, cstring file, int line ) {

      printf( "File: %s, Line: %d\n%s: %s\n", file.c_str(), line, logtype2cstring[ (size_t) logtype ].c_str(), message.c_str() );
}

void log_message( log_type logtype, const string& message, cstring file, int line ) {

      printf( "File: %s, Line: %d\n%s: %s\n", file.c_str(), line, logtype2cstring[ (size_t) logtype ].c_str(), message.c_str() );
}

void log_error( cstring file, int line ) {
  if( error::errorcstring.empty() ) {
    log_message( log_type::message, aserrorstring( error::errorstring ), file, line  );
  } else
    log_message( log_type::message, error::errorcstring, file, line );
}


}}


