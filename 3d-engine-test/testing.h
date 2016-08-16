#ifndef alexpolt_testing
#  define alexpolt_testing

#include "config.h"
#include <cassert>

namespace alexpolt { namespace testing {

  /*
    Testing facilities.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

#define TEST( EXPR ) \
assert( EXPR ); \
printf( #EXPR "...  %s\n", "passed" );

#define TEST_BEGIN( MODULE ) printf( "%s\nStart testing [" MODULE "]\n\n", config::appname );

#define TEST_END() printf( "\nTests passed successfully.\n" );

}}


#endif //alexpolt_testing

