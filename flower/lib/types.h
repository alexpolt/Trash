#pragma once
#include "macros.h"

using uint = unsigned;
using cchar = char const;
using cchar_ptr = cchar*;
using iid_t = unsigned const;

namespace lib {

  $t<$n T0> struct type_tag {};


  struct type_true {
    static const bool value = true;
  };

  struct type_false {
    static const bool value = false;
  };

  $t<$n T0>
  T0 declval();

  $t<$n...>
  using void_t = void;

  $t<bool N0, $n T1, $n T2>
  struct select {
    using type = T2;
  };

  $t<$n T1, $n T2>
  struct select< true, T1, T2 > {
    using type = T1;
  };

  $t<$n>
  struct is_template : type_false { };

  $t<$t1<$n...> class T0, $n... TT>
  struct is_template< T0< TT... > > : type_true { };

}

