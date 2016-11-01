#pragma once

//GNU

#ifdef __GNUG__

  #define $CONFIG_GCC

  #ifndef __EXCEPTIONS

    #define $CONFIG_NO_EXCEPT

  #endif

//MS

#elif defined( _MSC_VER )

  #define WIN32_LEAN_AND_MEAN
  
  #define $CONFIG_MSC

  #ifndef _CPPUNWIND

    #define $CONFIG_NO_EXCEPT

  #endif

#endif

//x64

#if defined( _LP64 ) || defined( _M_X64 )

  #define $CONFIG_64

#endif

//#define $CONFIG_NO_EXCEPT

//#define $CONFIG_NO_ATOMIC

#define $CONFIG_STL


#ifdef $CONFIG_STL
# include <vector>
# include <string>
#endif


namespace lib {

  namespace config {

    constexpr auto compilation_date = __DATE__ " " __TIME__;

    constexpr auto program_name = "Flower";

    constexpr auto program_version = 1;
  }

}


