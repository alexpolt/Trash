#ifndef alexpolt_defines
#  define alexpolt_defines

#include <stddef.h>
#ifdef ALXP_DEBUG
#include <cassert>
#endif

namespace alexpolt { namespace defines {

  /*
    Some defines.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */


#define default_copy_assign( Type ) \
    Type(const Type&) = default; \
    Type& operator=(const Type&) = default; 

#define default_copy_assign_move( Type ) \
    Type(Type&&) = default; \
    Type& operator=(Type&&) = default; 

#define delete_copy_assign( Type ) \
    Type(const Type&) = delete; \
    Type& operator=(const Type&) = delete; 

#define delete_copy_assign_move( Type ) \
    Type(Type&&) = delete; \
    Type& operator=(Type&&) = delete; 


#ifndef __clang__
#  define TLS thread_local 
#else
#  define TLS
#endif

#ifdef __GNUC__
#endif

#ifdef _WIN32
#endif

#ifdef _MSC_VER
#endif


}}


#endif //alexpolt_defines
