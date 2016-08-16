#ifndef alexpolt_error
#  define alexpolt_error

#include "config.h"
#include "usertypes/cstring.h"
#include "usertypes/string.h"
#include "logging.h"

namespace alexpolt { namespace error {

  /*
    Error handling.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  enum class code: unsigned {  failed = 0, success = 1 };

  extern TLS code errorcode;
  extern TLS cstring errorcstring;
  extern TLS char errorstring[ sizeof (string) ];

  //problems with TLS emulation in GCC - generates non TLS symbol in setError
  //extern TLS string errorstring; 
  //inline void setError( code code, string&& str ) {  checkClear(); errorcode = code; errorcstring = cnullstring; errorstring = util::move( str ); }

#define aserrorstring( value ) (*(string*)(void *)& value) //hack to solver problem with GCC MinGW 4.8.1

  inline void setErrorCode( code code ) {  errorcode = code;  }
  inline void setError( code code, cstring cstring ) {  errorcode = code; errorcstring = cstring;  }
  inline void setError( code code, string&& str ) {  
    errorcode = code; 
    errorcstring = cnullstring; 
    aserrorstring( errorstring ) = util::move( str ); 
  }

  inline void setSuccess( ) {  errorcode = code::success; }

  inline string getErrorString( ) {
    if( errorcstring.empty() ) 
      return aserrorstring( errorstring );
    else 
      return string{ errorcstring };
  }

  inline bool isSuccess() { return errorcode == code::success; }
  inline bool isFailed() { return errorcode == code::failed; }

  inline void logError( cstring file, int line ) { 
    if( isSuccess() ) return;
    logging::logError( file, line );
  }

}}

#endif // alexpolt_error

