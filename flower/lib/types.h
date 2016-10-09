#pragma once
#include "macros.h"

using uint = unsigned;
using cchar = char const;
using cstr = cchar*;
using iid_t = unsigned const;
using oid_t = unsigned const;

namespace lib {

  $T<$N T0> struct type_tag {};

  struct nocopy { 
    nocopy() { } 
    nocopy( nocopy const& ) = delete; 
    nocopy& operator=( nocopy const& ) = delete;
    nocopy( nocopy&& ) = default;
    nocopy& operator=( nocopy&& ) = default;
  };

  struct type_true { static const bool value = true; };
  struct type_false { static const bool value = false; };


  $T<$N...> struct type_first;
  $T<$N T0, $N... TT> struct type_first< T0, TT... > { using type = T0; };


  $T<$N T0> T0 declval();

  $T<$N...> using void_t = void;

  $T<$N T0> struct identity { using type = T0; };


  $T<bool N0, $N T1, $N T2>
  struct select { using type = T2; };

  $T<$N T1, $N T2>
  struct select< true, T1, T2 > { using type = T1; };


  $T<$N T0> struct is_ref : type_false { };
  $T<$N T0> struct is_ref< T0& > : type_true { };

  $T<$N T0> struct no_ref { using type = T0; };
  $T<$N T0> struct no_ref< T0& > { using type = T0; };


  $T<$N T0> 
  $N no_ref< T0 >::type&& move( T0&& value ) { 
      return static_cast< $N no_ref< T0 >::type && > ( value ); 
  }
  
  $T<$N T0> 
  $N select< is_ref< T0 >::value, T0, T0&& >::type 
  forward( $N no_ref< T0 >::type& value ) { 
      return static_cast< $N select< is_ref< T0 >::value, T0, T0&& >::type >( value ); 
  } 


  $T<$N>
  struct is_template : type_false { };

  $T< $T1<$N...> class T0, $N... TT >
  struct is_template< T0< TT... > > : type_true { using type = $N type_first< TT... >::type; };

  $T<$N T0>
  using template_arg_t = $N is_template< T0 >::type;

}

